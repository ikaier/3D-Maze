#include "FlashLight.h"


FlashLight::FlashLight()
{
	flashPos = glm::vec3(0.0f);
	flashDir = glm::vec3(0.0f);
}

FlashLight::FlashLight(glm::vec3 flashPos, glm::vec3 flashDir)
{
	flashShadowMap = new FlashLightShadowMap();
	flashShadowMap->Init(1024, 1024);
	this->flashPos = flashPos;
	this->flashDir = flashDir;
}


void FlashLight::SetFlashLight(glm::vec3 flashPos, glm::vec3 flashDir)
{
	this->flashPos = flashPos;
	this->flashDir = flashDir;
	lightProjection = glm::perspective<float>(glm::radians(14.5f),1.0f,near_plane, far_plane);
	lightView = glm::lookAt(flashPos, flashPos+flashDir, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}


void FlashLight::applyFlash(Shader& shader)
{
	shader.setFloat("spotLight.base.base.ambientIntensity", ambientIntensity);
	shader.setFloat("spotLight.base.base.diffuseIntensity", diffuseIntensity);
	shader.setFloat("spotLight.base.base.constant", constant);
	shader.setFloat("spotLight.base.base.linear", linear);
	shader.setFloat("spotLight.base.base.exponent", exponent);
	shader.setvec3("spotLight.base.base.color", glm::vec3(red, green, blue));
	shader.setvec3("spotLight.base.position", flashPos);
	shader.setvec3("spotLight.spotdirection", flashDir);
	//printf("%f,%f,%f\n", getCameraDirection().x, getCameraDirection().y, getCameraDirection().z);
	shader.setFloat("spotLight.edge", edge);
}

void FlashLight::flashLightOFF(Shader& shader)
{
	shader.setvec3("spotLight.base.base.color", glm::vec3(0.0f, 0.0f, 0.0f));

}
