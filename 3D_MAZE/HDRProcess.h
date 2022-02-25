#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

class HDRProcess
{
public:
	HDRProcess();
	void InitBloom(GLuint width, GLuint height);
	void WriteFloatBuffer();
	void ReadFloatBuffer(GLenum textureUnit, GLuint colorBufferIndex);
	void WriteColorBuffer(GLuint colorIndex);
	void ReadColorBuffer(GLenum textureUnit, GLuint colorBufferIndex);
private:
	GLuint hdrFBO,hdrRBODepth;
	GLuint width, height;
	GLuint colorBufferIDs[2];
	GLuint attachments[2];
	GLuint pingpongFBO[2];
	GLuint pingpongColorBIDS[2];
};

