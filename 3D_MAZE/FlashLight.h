#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"Shader.h"
#include"Texture.h"
#include"FlashLightShadowMap.h"
class FlashLight
{
public:
	FlashLight();
	FlashLight(glm::vec3 flashPos, glm::vec3 flashDir);
	void SetFlashLight(glm::vec3 flashPos,glm::vec3 flashDir);
	glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; };
	FlashLightShadowMap* GetShadowMap() { return flashShadowMap; };
	void applyFlash(Shader& shader);
	void flashLightOFF(Shader& shader);
	 ~FlashLight();
private:
	//flashlight cookie texture
	Texture* lighttexure;
	GLfloat red = 255.0f / 4;
	GLfloat green = 255.0f / 4;
	GLfloat blue = 255.0f /4;
	GLfloat ambientIntensity = 0.2f;
	GLfloat diffuseIntensity = 1.0f;
	GLfloat constant = 1.0f;
	GLfloat linear = 0.09f;
	GLfloat exponent = 0.32f;
	GLfloat edge = glm::cos(glm::radians(18.0f));
	GLuint shadowWidth = 1024, shadowHeight = 1024;
	float near_plane = 0.1f, far_plane = 10.0f;
	glm::mat4 lightProjection;
	glm::mat4 lightView, lightSpaceMatrix;
	glm::vec3 flashPos, flashDir;
	FlashLightShadowMap* flashShadowMap;

};

