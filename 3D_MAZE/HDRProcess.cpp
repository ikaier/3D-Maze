#include "HDRProcess.h"

HDRProcess::HDRProcess()
{
	GLuint hdrFBO = 0;
	GLuint textureID = 0;
	GLuint hdrRBODepth = 0;
}



void HDRProcess::CreateHDR(GLuint width, GLuint height)
{
	this->width = width;
	this->height = height;
	glGenFramebuffers(1, &hdrFBO);
	//floating point color buffer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//depth buffer
	glGenRenderbuffers(1, &hdrRBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	//attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBODepth);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer ERROR:%d\n", status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void HDRProcess::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
}

void HDRProcess::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}
