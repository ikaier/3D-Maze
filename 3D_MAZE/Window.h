#pragma once
#include<stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/quaternion.hpp>
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	void Initialize();
	bool getShouldClose();
	void swapBuffers();
	void processInput(GLfloat deltaTime);
	
	GLfloat* getsKeys();
	//glm::quat getQuatChange();
	GLfloat getXchange();
	GLfloat getYchange();

	GLfloat getBufferWidth();
	GLfloat getBufferHeight();
	
private:
	bool mouseFirstMove = true;
	GLfloat lastX, lastY, lastZ, offsetX, offsetY;
	glm::quat quatChange;

	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	GLfloat keys[5] = { 0.0f,0.0f,0.0f,0.0f,0.0f };

	static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
};

 