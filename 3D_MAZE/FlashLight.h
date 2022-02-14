#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"LightCube.h"
class FlashLight
{
public:
	FlashLight();
	FlashLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos,
		GLfloat xLightDir, GLfloat yLightDir, GLfloat zLightDIr);
	void applyFlash(GLuint spotLightPositionLocation, GLuint spotLighDirectiontLocation);

private:
	GLfloat xLightPos, yLightPos, zLightPos;
	GLfloat xLightDir, yLightDir, zLightDir;

};

