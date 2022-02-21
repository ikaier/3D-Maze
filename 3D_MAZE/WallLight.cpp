#include "WallLight.h"

WallLight::WallLight()
{
	wallLightsCount = 0;
}

WallLight::WallLight(std::vector<glm::vec3> wallLights, GLuint wallLightsCount)
	:wallLights(wallLights), wallLightsCount(wallLightsCount)
{
	//for (size_t i = 0; i < wallLightsCount; i++) {
	//	LightCube tempCube = LightCube(wallLights[i].x, wallLights[i].y, wallLights[i].z, 246.0f / 255.0f, 228.0f / 255.0f, 188.0f / 255.0f);
	//	cubes.push_back(tempCube);
	//}
}

//void WallLight::Apply(/*Shader& LightingShader,*/ GLuint positionLocation)
//{
//	glUniform3f(positionLocation, xLightPos, yLightPos, zLightPos);
//
//}

void WallLight::Draw(Shader& shader)
{
	for (size_t i = 0; i < wallLightsCount; i++) {
		//cubes[i].Draw(uniformLocation);
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		shader.setFloat(locBuff, ambientIntensity);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		shader.setFloat(locBuff, diffuseIntensity);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.constant", i);
		shader.setFloat(locBuff, constant);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.linear", i);
		shader.setFloat(locBuff, linear);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.exponent", i);
		shader.setFloat(locBuff, exponent);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		shader.setvec3(locBuff, glm::vec3(red, green, blue));

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		shader.setvec3(locBuff, wallLights[i]);
	}

}

WallLight::~WallLight()
{
}
