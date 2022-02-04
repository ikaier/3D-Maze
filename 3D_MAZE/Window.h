#pragma once
#include<stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	void Initialize();
	bool getShouldClose();
	void swapBuffers();
	void processInput();

	GLfloat getBufferWidth();
	GLfloat getBufferHeight();
	
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
};

 