#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"LightCube.h"
#include"Shader.h"

class WallLight
{
public:
	WallLight();
	WallLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos,
		GLfloat red, GLfloat green, GLfloat yellow,
		GLfloat AmbientIntensity, GLfloat DiffuseIntensity);
	void Apply(Shader& shader);
	~WallLight();
private:
	LightCube* aCube;
	GLfloat xLightPos, yLightPos, zLightPos, red, green, blue, AmbientIntensity, DiffuseIntensity;
};

