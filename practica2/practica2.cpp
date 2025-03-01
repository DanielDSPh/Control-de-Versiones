//Pr�ctica 2: �ndices, mesh, proyecciones, transformaciones geom�tricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crear�an ac�
static const char* frojo = "shaders/rojo.frag";
static const char* fverde = "shaders/verde.frag";
static const char* fazul = "shaders/azul.frag";
static const char* fcafe = "shaders/cafe.frag";
static const char* fverde_obscuro = "shaders/verde_obscuro.frag";

//color caf� en RGB : 0.478, 0.255, 0.067

//Pir�mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//V�rtices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	//Mis letras
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_letra_L[] = {
		//L
		-0.75f, 0.7f, 0.0f,		 0.3f,	1.0f,	0.5f,
		-0.9f, -0.7f, 0.0f,		 0.3f,	1.0f,	0.5f,
		-0.9f, 0.7f, 0.0f,		 0.3f,	1.0f,	0.5f,

		-0.75f, 0.7f, 0.0f,		0.3f,	1.0f,	0.5f,
		-0.9f, -0.7f, 0.0f,		0.3f,	1.0f,	0.5f,
		-0.75, -0.7f, 0.0f,		0.3f,	1.0f,	0.5f,

		-0.75f, -0.55f, 0.0f,	 0.3f,	1.0f,	0.5f,
		-0.75f, -0.7f, 0.0f,	0.3f,	1.0f,	0.5f,
		-0.4f, -0.55f, 0.0f,	0.3f,	1.0f,	0.5f,

		-0.75f, -0.7f, 0.0f,	 0.3f,	1.0f,	0.5f,
		-0.4f, -0.55f, 0.0f,	0.3f,	1.0f,	0.5f,
		-0.4f, -0.7f, 0.0f,		0.3f,	1.0f,	0.5f,
	};

	MeshColor* letra_L = new MeshColor();
	letra_L ->CreateMeshColor(vertices_letra_L, 12*6);
	meshColorList.push_back(letra_L);

	GLfloat vertices_letra_D[] = {
		//D
		-0.3f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.15f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.3f, -0.7f, 0.0f,		0.0f,	0.5f,	0.8f,

		-0.15f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.3f, -0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.15f, -0.7f, 0.0f,	0.0f,	0.5f,	0.8f,

		-0.15f, -0.7f, 0.0f,	0.0f,	0.5f,	0.8f,
		0.1f, -0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.1f, -0.55f, 0.0f,		0.0f,	0.5f,	0.8f,

		-0.15f, -0.7f, 0.0f,	0.0f,	0.5f,	0.8f,
		0.1f, -0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.15f, -0.55f, 0.0f,	0.0f,	0.5f,	0.8f,

		0.1f, -0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.1f, -0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.25f, -0.55f, 0.0f,	0.0f,	0.5f,	0.8f,

		0.1f, -0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.25f, -0.55f, 0.0f,	0.0f,	0.5f,	0.8f,
		0.1f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,

		0.25f, -0.55f, 0.0f,	0.0f,	0.5f,	0.8f,
		0.1f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.25f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,

		0.1f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.25f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.1f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,

		0.1f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.1f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.15f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,

		-0.15f, 0.7f, 0.0f,		0.0f,	0.5f,	0.8f,
		0.1f, 0.55f, 0.0f,		0.0f,	0.5f,	0.8f,
		-0.15f, 0.55f, 0.0f,	0.0f,	0.5f,	0.8f,
	};

	MeshColor* letra_D = new MeshColor();
	letra_D->CreateMeshColor(vertices_letra_D, 10 * 3 * 6);
	meshColorList.push_back(letra_D);

	GLfloat vertices_letra_R[] = {
		//R
		0.35f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.35f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.5f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.35f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.5f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.75f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, 0.55f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.75f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.75f, 0.55f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, 0.55f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.9f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.75f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.9f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.75f, 0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.9f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.75f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.9f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.9f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.9f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, 0.2f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.5f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.5f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.9f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.75f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,

		0.5f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.65f, 0.05f, 0.0f,		0.8f,	0.1f,	0.5f,
		0.9f, -0.7f, 0.0f,		0.8f,	0.1f,	0.5f,
	};

	MeshColor* letra_R = new MeshColor();
	letra_R->CreateMeshColor(vertices_letra_R, 10 * 3 * 6);
	meshColorList.push_back(letra_R);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar �ndices: objetos: cubo y  pir�mide [0]
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras [1]
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader_rojo = new Shader();//color rojo [2]
	shader_rojo->CreateFromFiles(vShader, frojo);
	shaderList.push_back(*shader_rojo);

	Shader* shader_verde = new Shader();//color verde [3]
	shader_verde->CreateFromFiles(vShader, fverde);
	shaderList.push_back(*shader_verde);

	Shader* shader_azul = new Shader();//color azul [4]
	shader_azul->CreateFromFiles(vShader, fazul);
	shaderList.push_back(*shader_azul);

	Shader* shader_cafe = new Shader();//color caf� [5]
	shader_cafe->CreateFromFiles(vShader, fcafe);
	shaderList.push_back(*shader_cafe);

	Shader* shader_verde_obscuro = new Shader();//color verde obscuro [6]
	shader_verde_obscuro->CreateFromFiles(vShader, fverde_obscuro);
	shaderList.push_back(*shader_verde_obscuro);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //�ndice 0 en MeshList
	CrearCubo();//�ndice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, �ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensi�n 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	
	//Model: Matriz de Dimensi�n 4x4 en la cual se almacena la multiplicaci�n de las transformaciones geom�tricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		//Comentar la parte de las letras (desde aqu�) para visualizar el dibujo de la casa.
		//Esta es la parte de las letras:
		
		//Para las letras hay que usar el segundo set de shaders con �ndice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Letra L
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		//Letra D
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		//Letra R
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();
		//Termina la parte de las letras. Comentar y descomentar hasta aqu� para las letras
		



		//Comentar esta parte para observar las letras
		//Descomentar para observar el dibujo de la casa
		/*
		//Casa. Cubo Rojo
		shaderList[2].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.275f, -6.0f));
		model = glm::scale(model, glm::vec3(1.05f, 1.25f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo una vez
		meshList[1]->RenderMesh();

		//Casa. Piramide Azul
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, -6.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo una vez
		meshList[0]->RenderMesh();

		//Casa. Cubos Verdes
		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, 0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo una vez
		meshList[1]->RenderMesh();

		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		shaderList[3].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.725f, -4.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		//�rboles. Piramides Verde Obscuro
		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.4f, -6.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo una vez
		meshList[0]->RenderMesh();

		shaderList[6].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.4f, -6.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[0]->RenderMesh();

		//�rboles. Cubos Caf�
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.8f, -6.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); //Solo una vez
		meshList[1]->RenderMesh();

		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.8f, -6.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();
		//Aqui termina la creacion del dibujo. Comentar o descomentar hasta aqu�
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
