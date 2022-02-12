#include "WallLight.h"

WallLight::WallLight()
{
	xLightPos = 0.0f;
	yLightPos = 0.0f;
	zLightPos = 0.0f;

}

WallLight::WallLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos)
	:xLightPos(xLightPos),yLightPos(yLightPos),zLightPos(zLightPos)
{
	aCube = new LightCube( xLightPos,  yLightPos,  zLightPos, 246.0f / 255.0f, 228.0f / 255.0f, 188.0f / 255.0f);
}

void WallLight::Apply(/*Shader& LightingShader,*/ GLuint positionLocation)
{

	glUniform3f(positionLocation, xLightPos, yLightPos, zLightPos);

}

void WallLight::DrawCubes(GLuint positionLocation)
{
	aCube->Draw(positionLocation);
}

WallLight::~WallLight()
{
}
