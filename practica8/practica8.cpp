/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture paredTexture;

//Partes del Carro
Model Auto;
Model Llanta_derecha_atras;
Model Llanta_derecha_frontal;
Model Llanta_izquierda_atras;
Model Llanta_izquierda_frontal;
Model Cofre;

//Cristal
Model Cristal;

Model Blackhawk_M;
Model Lampara;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight



// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,


	};

	unsigned int wallIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat wallVertices[] = {
		10.0f, 40.0f, -30.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		10.0f, 40.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		10.0f, 0.0f, -30.0f,	0.0f, 4.0f,		1.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 0.0f,		1.0f, 4.0f,		1.0f, 0.0f, 0.0f
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(wallVertices, wallIndices, 64, 12);
	meshList.push_back(obj5);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	paredTexture = Texture("Textures/pared_texture.jpg");
	paredTexture.LoadTexture();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Lampara = Model();
	Lampara.LoadModel("Models/lampara_model.obj");

	//Carro
	Llanta_derecha_atras = Model();
	Llanta_derecha_atras.LoadModel("Models/llanta_der_a_texture.obj");
	Llanta_derecha_frontal = Model();
	Llanta_derecha_frontal.LoadModel("Models/llanta_der_f_texture.obj");
	Llanta_izquierda_atras = Model();
	Llanta_izquierda_atras.LoadModel("Models/llanta_izq_a_texture.obj");
	Llanta_izquierda_frontal = Model();
	Llanta_izquierda_frontal.LoadModel("Models/llanta_izq_f_texture.obj");
	Auto = Model();
	Auto.LoadModel("Models/auto_texture.obj");
	Cofre = Model();
	Cofre.LoadModel("Models/cofre_texture.obj");

	//Cristal
	Cristal = Model();
	Cristal.LoadModel("Models/cristal.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales

	PointLight pointLights_1[MAX_POINT_LIGHTS];
	unsigned int pointLightCount_1 = 0;

	//Luz de la lampara
	pointLights_1[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.4f, 4.0f,
		-4.0f, 4.0f, 4.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount_1++;

	//Luz del cristal
	pointLights_1[1] = PointLight(0.47f, 0.07f, 0.47f,
		1.0f, 4.0f,
		4.0f, 4.0f, 4.0f,
		0.2f, 0.0f, 0.1f);
	pointLightCount_1++;


	PointLight pointLights_2[MAX_POINT_LIGHTS];
	unsigned int pointLightCount_2 = 0;
	
	pointLights_2[0] = pointLights_1[1];
	pointLightCount_2++;
	
	pointLights_2[1] = pointLights_1[0];
	pointLightCount_2++;
	


	SpotLight spotLights_1[MAX_SPOT_LIGHTS];
	unsigned int spotLightCount_1 = 0;
		
	//luz cofre
	spotLights_1[0] = SpotLight(1.0f, 0.5f, 0.2f,
		1.0f, 1.0f,
		10.85f, 3.0f, -15.2f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount_1++;

	//luz frontal coche
	spotLights_1[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		10.85f, 2.0f, -15.2f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount_1++;

	//luz trasera coche
	spotLights_1[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		-11.1f, 2.0f, -15.2f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount_1++;

	SpotLight spotLights_2[MAX_SPOT_LIGHTS];
	unsigned int spotLightCount_2 = 0;

	//luz cofre
	spotLights_2[0] = spotLights_1[0];
	spotLightCount_2++;

	//luz trasera coche
	spotLights_2[1] = spotLights_1[2];
	spotLightCount_2++;

	//luz frontal coche
	spotLights_2[2] = spotLights_1[1];
	spotLightCount_2++;
	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		if (mainWindow.get_encendido_lampara() && mainWindow.get_encendido_cristal()) {
			shaderList[0].SetPointLights(pointLights_1, pointLightCount_1);
		}
		else if(mainWindow.get_encendido_lampara() && !mainWindow.get_encendido_cristal()){
			shaderList[0].SetPointLights(pointLights_1, pointLightCount_1 - 1);
		}
		else if (!mainWindow.get_encendido_lampara() && mainWindow.get_encendido_cristal()) {
			shaderList[0].SetPointLights(pointLights_2, pointLightCount_2 - 1);
		}
		else {
			shaderList[0].SetPointLights(pointLights_1, pointLightCount_1 - 2);
		}
		
		if (mainWindow.isMovingForward()) {
			shaderList[0].SetSpotLights(spotLights_1, spotLightCount_1 - 1);
			spotLights_1[1].SetFlash(glm::vec3(mainWindow.getmovimiento() + 10.85, 2.0f, -15.2f), glm::vec3(1.0f, 0.0f, 0.0f));

			//Luz del cofre
			float angleDeg = mainWindow.getarticulacion_cofre();
			float angleRad = glm::radians(angleDeg);
			glm::vec3 direction = glm::normalize(glm::vec3(cos(angleRad), sin(angleRad), 0.0f));
			spotLights_1[0].SetFlash(glm::vec3(mainWindow.getmovimiento() + 7.85f, 3.0f, -15.2f), direction);
		}
		else {
			shaderList[0].SetSpotLights(spotLights_2, spotLightCount_2 - 1);
			spotLights_2[1].SetFlash(glm::vec3(mainWindow.getmovimiento() - 11.1f, 2.0f, -15.2f), glm::vec3(-1.0f, 0.0f, 0.0f));

			//Luz del cofre
			float angleDeg = mainWindow.getarticulacion_cofre();
			float angleRad = glm::radians(angleDeg);
			glm::vec3 direction = glm::normalize(glm::vec3(cos(angleRad), sin(angleRad), 0.0f));
			spotLights_2[0].SetFlash(glm::vec3(mainWindow.getmovimiento() + 7.85f, 3.0f, -15.2f), direction);
		}
		

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pared
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.23f, -15.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmovimiento(), 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto.RenderModel();

		
		//Llanta delantera izquierda
		modelaux = model;
		model = glm::translate(model, glm::vec3(6.9f, 0.91f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_frontal.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.5f, 0.9f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_izquierda_frontal.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(6.9f, 0.91f, 3.45f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_frontal.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.5f, 0.91f, 3.45f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_derecha_atras.RenderModel();

		//Cofre
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		model = glm::translate(model, glm::vec3(4.3f, 3.55f, -0.23f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion_cofre()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();

		
		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 4.0f, 4.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Cristal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 4.0f, 4.0));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cristal.RenderModel();
				
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
