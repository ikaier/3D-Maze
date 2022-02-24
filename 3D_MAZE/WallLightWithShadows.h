#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>
#include"OmniShadowMap.h"
class WallLightWithShadows
	
{
public:
	//Pre-generate n omnishadowmap, modify them on the fly
	WallLightWithShadows();
	WallLightWithShadows(std::vector<glm::vec3> wallLights, GLuint wallLightsCount,GLuint SetWLShadowNumber);
	GLuint GetSetWLShadowNumber() { return SetWLShadowNumber; };
	std::vector<glm::vec3> GetWLShadow(glm::vec3 cameraPosition);
	std::vector<OmniShadowMap> GetShadowMap() { return wallLightsWithShadows; };
private:
	
	glm::vec3 cameraPosition;
	std::vector<glm::vec3> wallLights;
	std::vector<OmniShadowMap> wallLightsWithShadows;
	GLuint wallLightsCount;
	GLuint SetWLShadowNumber;
	std::vector<glm::vec3> quickSelect(std::vector<GLfloat> &wallLights);
	GLuint partition(std::vector<GLfloat> &wallLights, GLuint left, GLuint right);
	GLuint choosePivot( GLuint left, GLuint right);
	std::vector<glm::vec3> quickSelect(std::vector<glm::vec3>& wallLights);
	GLuint partition(std::vector<glm::vec3>& wallLights, GLuint left, GLuint right);
	
	GLfloat squareDistance(glm::vec3 wallLight);
	void printVector(std::vector<GLfloat> vec);
};

