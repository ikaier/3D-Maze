#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"LightCube.h"


class WallLight
{
public:
	WallLight();
	WallLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos);
	void Apply(/*Shader& LightingShader,*/ GLuint positionLocation);
	void DrawCubes(GLuint positionLocation);
	~WallLight();
private:
	LightCube* aCube;
	GLfloat xLightPos, yLightPos, zLightPos;
	//Wall light property

		
};

