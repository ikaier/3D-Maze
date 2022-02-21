#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>

#include"Shader.h"


class WallLight
{
public:
	WallLight();
	WallLight(std::vector<glm::vec3> wallLights, GLuint wallLightsCount);
	//void Apply(/*Shader& LightingShader,*/ GLuint positionLocation);
	void Draw(Shader& shader);
	~WallLight();
private:
	//std::vector<LightCube> cubes;
	std::vector<glm::vec3> wallLights;
	GLuint wallLightsCount;

	//Wall light property
	GLfloat red = 246.0f / 255.0f;
	GLfloat green = 228.0f / 255.0f;
	GLfloat blue = 188.0f / 255.0f;
	GLfloat ambientIntensity = 0.2f;
	GLfloat diffuseIntensity = 0.3f;
	GLfloat constant = 1.0f;
	GLfloat linear = 0.35f;
	GLfloat exponent = 1.8f;
		
};

