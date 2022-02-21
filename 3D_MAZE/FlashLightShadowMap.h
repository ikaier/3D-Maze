#pragma once
#include<stdio.h>
#include<GL/glew.h>

class FlashLightShadowMap
{
public:
	FlashLightShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }
	~FlashLightShadowMap();
protected:
	GLuint FBO, depthMap;
	GLuint shadowWidth, shadowHeight;
};

