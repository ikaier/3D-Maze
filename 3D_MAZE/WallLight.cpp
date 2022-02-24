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
	lightingCubeShader.CreateFromFiles("Shaders/Lightingshader.vert", "Shaders/Lightingshader.frag");
	for (size_t i = 0; i < wallLightsCount; i++) {
		LightCube tempcube = LightCube(wallLights[i].x, wallLights[i].y, wallLights[i].z);
		tempcube.Draw(lightingCubeShader.GetTransformLocation());
		lightCubes.push_back(tempcube);
	}
}

//void WallLight::Apply(/*Shader& LightingShader,*/ GLuint positionLocation)
//{
//	glUniform3f(positionLocation, xLightPos, yLightPos, zLightPos);
//
//}


void WallLight::Draw(Shader& shader)
{
	if (wallLightsCount > MAX_POINT_LIGHTS) wallLightsCount = MAX_POINT_LIGHTS;
	shader.setInt("pointLightCount", wallLightsCount);
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

void WallLight::DrawLightCubes(glm::mat4 projection, glm::mat4 view)
{
	lightingCubeShader.UseShader();
	lightingCubeShader.setMat4("projection", projection);
	lightingCubeShader.setMat4("view", view);
	lightingCubeShader.setvec4("Color", glm::vec4(red + (1 - red) * 0.3f, green + (1 - green) * 0.3f, blue + (1 - blue) * 0.3f, 0.8f));
	for (size_t i = 0; i < wallLightsCount; i++) {
		lightCubes[i].Draw(lightingCubeShader.GetTransformLocation());
	}
	glUseProgram(0);
}



WallLight::~WallLight()
{
}
