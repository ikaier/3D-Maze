#include "WallLight.h"

WallLight::WallLight()
{
	xLightPos = 0.0f;
	yLightPos = 0.0f;
	zLightPos = 0.0f;
	red = 0.0f;
	green = 0.0f;
	blue = 0.0f;
	AmbientIntensity = 1.0f;
	DiffuseIntensity = 1.0f;
}

WallLight::WallLight(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos, 
	GLfloat red, GLfloat green, GLfloat blue, 
	 GLfloat DiffuseIntensity)
	:xLightPos(xLightPos),yLightPos(yLightPos),zLightPos(zLightPos),
	red(red),green(green),blue(blue), 
	 DiffuseIntensity(DiffuseIntensity)
{
	assert(red >= 0.0f && red <= 1.0f && green >= 0.0f && green <= 1.0f && blue >= 0.0f && blue <= 1.0f);
	aCube = new LightCube( xLightPos,  yLightPos,  zLightPos,  red,  green,  blue);
}

void WallLight::Apply(Shader& LightingShader)
{
	
	LightingShader.setvec4("Color", glm::vec4(red+(1-red)*0.3f, green + (1 - green) * 0.3f, blue + (1 - blue) * 0.3f, 0.8f));
	aCube->Draw(LightingShader.GetTransformLocation());
	
}

WallLight::~WallLight()
{
}
