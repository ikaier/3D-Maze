#include "FlashLight.h"

FlashLight::FlashLight()
{
	xLightPos = 0.0f;
	yLightPos = 0.0f;
	zLightPos = 0.0f;

	xLightDir = 0.0f;
	yLightDir = 0.0f;
	zLightDir = 0.0f;
}

FlashLight::FlashLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos, GLfloat xLightDir, GLfloat yLightDir, GLfloat zLightDIr)
	:xLightPos(xLightPos), yLightPos(yLightPos), zLightPos(zLightPos),
	xLightDir(xLightDir), yLightDir(yLightDir), zLightDir(yLightDir)
{

}

void FlashLight::applyFlash(GLuint spotLightPositionLocation, GLuint spotLighDirectiontLocation)
{

}
