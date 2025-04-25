/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

//variables para animación
bool avanza;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float angulovaria = 0.0f;
float angulo_alas = 0.0f;
bool alas_bajan = true;
float mov_dragon = 0.0f;
float giro_dragon = 0.0f;
int retardo = 0;
int use_texture = 0;

//Variables para las cabezas
float t_cabeza1 = 0.0f;
float t_cabeza2 = 0.0f;
float t_cabeza3 = 0.0f;
float t_cabeza5 = 0.0f;
float amplitud_cabeza1 = 0.02f;
float amplitud_cabeza2 = 0.2f;
float amplitud_cabeza3 = 0.3f;
float angulo_cabeza4 = 0.0f;
float amplitud_cabeza5 = 0.4f;

//Constantes de distancia entre letras de la tipografía
const float dist_u = 0.123f;
const float dist_v = -0.243f;
const int numero_letras = 27;

//Más para las letras
int estados_letra[23][2] = {
	{2, 3}, //
	{7, 1}, //P
	{1, 2}, //R
	{6, 1}, //O
	{0, 3}, //Y
	{4, 0}, //E
	{2, 0}, //C
	{3, 2}, //T
	{6, 1}, //O
	{2, 3}, //
	{2, 0}, //C
	{6, 0}, //G
	{4, 0}, //E
	{0, 1}, //I
	{7, 0}, //H
	{2, 0}, //C
	{2, 3}, //
	{5, 0}, //F
	{4, 0}, //E
	{1, 2}, //R
	{0, 1}, //I
	{0, 0}, //A
	{2, 3}  //
};
float clk_letras = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

Texture Tipografia;
Texture Fuego;
Texture Hielo;
Texture Gas;
Texture Rayo;
Texture Arena;

Model Dragon_M;
Model Dragon_Ala_Derecha;
Model Dragon_Ala_Izquierda;
Model Dragon_nocabeza;
Model Cabeza_1;
Model Cabeza_2;
Model Cabeza_3;
Model Cabeza_4;
Model Cabeza_5;

Model Arco;
Model Cartel;
Model Reja;

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
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//funciones movimieto de cabezas

//Cabeza 1
float espiral_arquimides_x(float t, float a) {
	float fase = fmod(t, 1.0f);
	float t_norm = fase < 0.5f ? fase * 2.0f : (1.0f - fase) * 2.0f;
	float arquimides_t = t_norm * (4.0f * M_PI);

	return a * arquimides_t * cos(arquimides_t);
}
float espiral_arquimides_z(float t, float a) {
	float fase = fmod(t, 1.0f);
	float t_norm = fase < 0.5f ? fase * 2.0f : (1.0f - fase) * 2.0f;
	float arquimides_t = t_norm * (4.0f * M_PI);

	return a * arquimides_t * sin(arquimides_t);
}

//Cabeza 2
float cabeza2_desplazamiento_y(float t, float amplitud) {
	return sin(t) * amplitud;
}
float cabeza2_desplazamiento_z(float t, float amplitud) {
	return sin(t * 2.0f) * (amplitud * 0.6f);
}

//Cabeza 3
float ruido_suave(float t) {
	return sin(t) * 0.5f + sin(t * 2.4f + 1.0f) * 0.25f + sin(t * 3.7f + 2.0f) * 0.15f;
}
float cabeza3_desplazamiento_y(float t, float amplitud) {
	return ruido_suave(t) * amplitud;
}
float cabeza3_desplazamiento_z(float t, float amplitud) {
	return ruido_suave(t + 1.5f) * (amplitud * 0.7f);
}

//Cabeza 5
float lemniscata_y(float t, float a) {
	return a * sin(t) * cos(t);
}
float lemniscata_z(float t, float a) {
	return a * sin(t);
}


//Calculo de Offset
glm::vec2 calcularOffset(int clk) {
	int indice = clk;

	if (indice < 0 || indice >= 23) {
		return glm::vec2(estados_letra[0][0] * dist_u,
			estados_letra[0][1] * dist_v);
	}
	
	return glm::vec2(estados_letra[indice][0] * dist_u,
		estados_letra[indice][1] * dist_v);
}


//cálculo del promedio de las normales para sombreado de Phong
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

		
	unsigned int cartelIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};
	GLfloat cartelVertices[] = {
		1.5f, 0.0f,	 0.675f,		0.039f, 0.791f,		0.0f, -1.0f, 0.0f,
		2.25f, 0.0f, 0.675f,		0.091f, 0.791f,		0.0f, -1.0f, 0.0f,
		2.25f, 0.0f, -0.675f,		0.091f, 0.881f,		0.0f, -1.0f, 0.0f,
		1.5f, 0.0f, -0.675f,		0.039f, 0.881f,		0.0f, -1.0f, 0.0f,

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
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(cartelVertices, cartelIndices, 32, 6);
	meshList.push_back(obj8);


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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tipografia = Texture("Textures/crash_tipografia.png");
	Tipografia.LoadTextureA();
	Fuego = Texture("Textures/fuego.png");
	Fuego.LoadTextureA();
	Hielo = Texture("Textures/hielo.png");
	Hielo.LoadTextureA();
	Gas = Texture("Textures/venenoso.png");
	Gas.LoadTextureA();
	Gas = Texture("Textures/venenoso.png");
	Gas.LoadTextureA();
	Rayo = Texture("Textures/rayos.png");
	Rayo.LoadTextureA();
	Arena = Texture("Textures/arena.png");
	Arena.LoadTextureA();

	Dragon_M = Model();
	Dragon_M.LoadModel("Models/dragon_cuerpo_sincabezas.obj");
	Dragon_Ala_Derecha = Model();
	Dragon_Ala_Derecha.LoadModel("Models/dragon_ala_derecha.obj");
	Dragon_Ala_Izquierda = Model();
	Dragon_Ala_Izquierda.LoadModel("Models/dragon_ala_izquierda.obj");

	Dragon_nocabeza = Model();
	Dragon_nocabeza.LoadModel("Models/dragon_cuerpo_sincabezas.obj");
	Cabeza_1 = Model();
	Cabeza_1.LoadModel("Models/cabeza_1.obj");
	Cabeza_2 = Model();
	Cabeza_2.LoadModel("Models/cabeza_2.obj");
	Cabeza_3 = Model();
	Cabeza_3.LoadModel("Models/cabeza_3.obj");
	Cabeza_4 = Model();
	Cabeza_4.LoadModel("Models/cabeza_4.obj");
	Cabeza_5 = Model();
	Cabeza_5.LoadModel("Models/cabeza_5.obj");

	Arco = Model();
	Arco.LoadModel("Models/arco.obj");
	Cartel = Model();
	Cartel.LoadModel("Models/cartel.obj");
	Reja= Model();
	Reja.LoadModel("Models/reja.obj");

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
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	mov_dragon = 0.0f;
	avanza = true;
	angulo_alas = 0.0f;
	alas_bajan = true;
	giro_dragon = 0.0f;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//CLK Letras
		clk_letras += 0.05f * deltaTime;

		angulovaria += 0.05f * deltaTime;
		//cabeza 1
		t_cabeza1 += 0.002f * deltaTime;
		//cabeza 2
		t_cabeza2 += 0.02f * deltaTime;
		//cabeza 3
		t_cabeza3 += 0.05f * deltaTime;
		//cabeza 4
		angulo_cabeza4 += 0.02f * deltaTime;
		//cabeza 5
		t_cabeza5 += 0.05f * deltaTime;
		
		if (alas_bajan) {
			if (angulo_alas < 1) {
				angulo_alas += 0.02f * deltaTime;
			}else{
				alas_bajan = false;
			}
		}
		else {
			if (angulo_alas > -0.55) {
				angulo_alas -= 0.02f * deltaTime;
			}
			else {
				alas_bajan = true;
			}
		}

		if (avanza) {
			if (mov_dragon > -70) {
				mov_dragon -= 0.1 * deltaTime;
			}
			else {
				avanza = false;
				giro_dragon = 180.0f;
			}
		}
		else {
			if (mov_dragon < 50) {
				mov_dragon += 0.1 * deltaTime;
			}
			else {
				avanza = true;
				giro_dragon = 0.0f;
			}
		}


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime*0.5);
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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 model_cabeza1(1.0);
		glm::mat4 model_cabeza2(1.0);
		glm::mat4 model_cabeza3(1.0);
		glm::mat4 model_cabeza4(1.0);
		glm::mat4 model_cabeza5(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Dragón
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mov_dragon, 5.0f+3*sin(glm::radians(angulovaria*3.0)), 6.0));
		model = glm::rotate(model, giro_dragon * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.4f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();

		//Ala Izquierda Dragón
		modelaux = model;
		model = model = glm::translate(model, glm::vec3(-0.575f, 3.33f, 0.28));
		model = glm::rotate(model, angulo_alas, glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala_Izquierda.RenderModel();

		//Ala Derecha Dragón
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-0.71f, 3.3f, -0.49));
		model = glm::rotate(model, angulo_alas, glm::vec3(-1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala_Derecha.RenderModel();

		//Cabeza 1
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-1.934f + espiral_arquimides_x(t_cabeza1, amplitud_cabeza1), 3.673f, 
			-0.701f + espiral_arquimides_z(t_cabeza1, amplitud_cabeza1)));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza_1.RenderModel();
		model_cabeza1 = model;

		//Cabeza 2
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-2.056f, 3.601f + cabeza2_desplazamiento_y(t_cabeza2, amplitud_cabeza2), 
			0.314f + cabeza2_desplazamiento_z(t_cabeza2, amplitud_cabeza2)));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza_2.RenderModel();
		model_cabeza2 = model;

		//Cabeza 3
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-2.34f, 2.656f + cabeza3_desplazamiento_y(t_cabeza3, amplitud_cabeza3), 
			-0.969f + cabeza3_desplazamiento_z(t_cabeza3, amplitud_cabeza3)));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza_3.RenderModel();
		model_cabeza3 = model;
		
		//Cabeza 4
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-2.576f, 2.998f, -0.242f));
		model = glm::rotate(model, 0.6f+cos(angulo_cabeza4), glm::vec3(-1.0f, 1.0f, 0.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza_4.RenderModel();
		model_cabeza4 = model;

		//Cabeza 5
		model = modelaux;
		model = model = glm::translate(model, glm::vec3(-2.401f, 2.701f + lemniscata_y(t_cabeza5, amplitud_cabeza5), 
			0.514f + lemniscata_z(t_cabeza5, amplitud_cabeza5*0.5)));
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza_5.RenderModel();
		model_cabeza5 = model;

		//Arco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -20.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco.RenderModel();
		modelaux = model;

		//Reja
		model = modelaux;
		model = glm::translate(model, glm::vec3(-6.653f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();

		model = modelaux;
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-6.653f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Reja.RenderModel();
		
		//Cartel
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 14.645f, 1.704f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cartel.RenderModel();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Creación del Cartel con movimiento en las letras
		for (int i = 0; i < 6; i++) {
			toffset = calcularOffset(clk_letras - i);
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 14.645f, 1.71f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			model = glm::translate(model, glm::vec3(-0.75f*i, 0.0f, 0.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			Tipografia.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[7]->RenderMesh();
		}

		//Fuego
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = model_cabeza1;
		model = glm::translate(model, glm::vec3(-3.0f, 1.2f, -2.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, mainWindow.isPressY() ? glm::vec3(3.0f) : glm::vec3(0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Fuego.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Rayos
		model = model_cabeza2;
		model = glm::translate(model, glm::vec3(-2.0f, 1.4f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, mainWindow.isPressU() ? glm::vec3(3.0f) : glm::vec3(0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Rayo.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Gas Venenoso
		model = model_cabeza3;
		model = glm::translate(model, glm::vec3(-3.8f, 0.3f, -2.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, mainWindow.isPressI() ? glm::vec3(3.0f) : glm::vec3(0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Gas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Hielo
		model = model_cabeza4;
		model = glm::translate(model, glm::vec3(-3.9f, 0.0f, -1.0f));
		model = glm::rotate(model, 105 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, mainWindow.isPressO() ? glm::vec3(3.0f) : glm::vec3(0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Hielo.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//Arena
		model = model_cabeza5;
		model = glm::translate(model, glm::vec3(-3.3f, -1.9f, 2.1f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, mainWindow.isPressP() ? glm::vec3(3.0f) : glm::vec3(0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Arena.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		if (clk_letras > numero_letras) {
			clk_letras = 0;
		}

		glDisable(GL_BLEND);
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
