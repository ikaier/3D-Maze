#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>

#include"Shader.h"
#include"LightCube.h"
#include"OmniShadowMap.h"


class WallLight
{
public:
	WallLight();
	WallLight(std::vector<glm::vec3> wallLights, GLuint wallLightsCount, GLuint SetWLShadowNumber);
	std::vector<glm::vec3> GetWLShadow(glm::vec3 cameraPosition);
	
	GLuint GetSetWLShadowNumber() { return SetWLShadowNumber; };
	std::vector<OmniShadowMap> GetShadowMap() { return wallLightsWithShadows; };
	//void Apply(/*Shader& LightingShader,*/ GLuint positionLocation);
	void Draw(Shader& shader);
	void DrawLightCubes(glm::mat4 projection,glm::mat4 view);
	~WallLight();
private:
	//std::vector<LightCube> cubes;
	GLuint SetWLShadowNumber;
	std::vector<OmniShadowMap> wallLightsWithShadows;
	glm::vec3 cameraPosition;
	Shader lightingCubeShader;
	std::vector<glm::vec3> wallLights;
	std::vector<LightCube>lightCubes;
	GLuint wallLightsCount;


	//Wall light property
	GLfloat red = 246.0f/2 ;
	GLfloat green = 228.0f/2 ;
	GLfloat blue = 188.0f/2 ;
	GLfloat ambientIntensity = 0.01f;
	GLfloat diffuseIntensity = 0.3f;
	GLfloat constant = 1.0f;
	GLfloat linear = 0.8f;
	GLfloat exponent = 6.4f;
		
	std::vector<glm::vec3> quickSelect(std::vector<glm::vec3>& wallLights);
	GLuint partition(std::vector<glm::vec3>& wallLights, GLuint left, GLuint right);
	GLuint choosePivot(GLuint left, GLuint right);
	GLfloat squareDistance(glm::vec3 wallLight);
};

