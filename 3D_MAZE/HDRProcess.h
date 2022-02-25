#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

class HDRProcess
{
public:
	HDRProcess();
	void CreateHDR(GLuint width, GLuint height);
	void Write();
	void Read(GLenum textureUnit);
private:
	GLuint hdrFBO, textureID,hdrRBODepth;
	GLuint width, height;
};

