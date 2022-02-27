#include "WallLight.h"

WallLight::WallLight()
{
	wallLightsCount = 0;
}

WallLight::WallLight(std::vector<glm::vec3> wallLights, GLuint wallLightsCount, GLuint SetWLShadowNumber)
	:wallLights(wallLights), wallLightsCount(wallLightsCount), SetWLShadowNumber(SetWLShadowNumber)
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
	if (wallLightsCount < SetWLShadowNumber)
	{
		SetWLShadowNumber = wallLightsCount;
	}
	for (size_t i = 0; i < SetWLShadowNumber; i++) {
		OmniShadowMap* temp = new OmniShadowMap();
		temp->Init(1024, 1024);
		wallLightsWithShadows.push_back(*temp);
	}
}


std::vector<glm::vec3> WallLight::GetWLShadow(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	wallLights = quickSelect(wallLights);

	return wallLights;
}
std::vector<glm::vec3> WallLight::quickSelect(std::vector<glm::vec3>& wallLights)
{
	GLuint left = 0;
	GLuint right = wallLightsCount - 1;
	GLuint pivotIndex = wallLightsCount;
	while (pivotIndex != SetWLShadowNumber) {
		pivotIndex = partition(wallLights, left, right);//problem here 
		if (pivotIndex < SetWLShadowNumber) {
			left = pivotIndex;
		}
		else {
			right = pivotIndex - 1;
		}
	}
	return wallLights;
}

GLuint WallLight::partition(std::vector<glm::vec3>& wallLights, GLuint left, GLuint right)
{
	GLuint pivot = choosePivot(left, right);
	GLfloat pivotDistance = squareDistance(wallLights[pivot]);
	while (left < right) {
		if (squareDistance(wallLights[left]) >= pivotDistance) {
			std::swap(wallLights[left], wallLights[right]);

			right--;
		}
		else {
			left++;
		}
	}
	if (squareDistance(wallLights[left]) < pivotDistance) {
		left++;
	}
	return left;
}

GLuint WallLight::choosePivot(GLuint left, GLuint right)
{
	return left + (right - left) / 2;
}

GLfloat WallLight::squareDistance(glm::vec3 wallLight)
{
	return glm::pow(wallLight.x - cameraPosition.x,2) + glm::pow(wallLight.z - cameraPosition.z,2);
}

void WallLight::Draw(Shader& shader)
{
	if (SetWLShadowNumber > MAX_POINT_LIGHTS) SetWLShadowNumber = MAX_POINT_LIGHTS;
	shader.setInt("pointLightCount", SetWLShadowNumber);
	for (size_t i = 0; i < SetWLShadowNumber; i++) {
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
