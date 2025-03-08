#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getrotaw() { return rotaw; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }

	GLfloat getarticulacion_cola_1() { return articulacion_cola_1; }
	GLfloat getarticulacion_cola_2() { return articulacion_cola_2; }
	GLfloat getarticulacion_pata_1() { return articulacion_pata_1; }
	GLfloat getarticulacion_planta_1() { return articulacion_planta_1; }
	GLfloat getarticulacion_pata_2() { return articulacion_pata_2; }
	GLfloat getarticulacion_planta_2() { return articulacion_planta_2; }
	GLfloat getarticulacion_manita_1() { return articulacion_manita_1; }
	GLfloat getarticulacion_munneca_1() { return articulacion_munneca_1; }
	GLfloat getarticulacion_manita_2() { return articulacion_manita_2; }
	GLfloat getarticulacion_munneca_2() { return articulacion_munneca_2; }
	GLfloat getarticulacion_mandibula_inferior() { return articulacion_mandibula_inferior; }
	GLfloat getarticulacion_mandibula_superior() { return articulacion_mandibula_superior; }
	

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, rotaw, articulacion1, articulacion2, articulacion3, articulacion4, articulacion_cola_1, articulacion_cola_2, 
		articulacion_pata_1, articulacion_planta_1, articulacion_pata_2, articulacion_planta_2, articulacion_manita_1, articulacion_munneca_1,
		articulacion_manita_2, articulacion_munneca_2, articulacion_mandibula_inferior, articulacion_mandibula_superior;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

