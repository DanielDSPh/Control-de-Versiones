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
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat get_mov_helicoptero() { return mov_helicoptero; }
	GLfloat get_encendido_lampara() { return encendido_lampara; }
	GLfloat get_encendido_cristal() { return encendido_cristal; }

	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getmovimiento() { return movimiento; }
	GLfloat getarticulacion_cofre() { return articulacion_cofre; }
	GLfloat isMovingForward() { return moving_forward; }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat mov_helicoptero;
	GLboolean encendido_lampara, encendido_cristal, moving_forward;
	GLfloat articulacion1, articulacion2, articulacion3, articulacion4, movimiento, articulacion_cofre;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

