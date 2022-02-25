#include "HDRProcess.h"

HDRProcess::HDRProcess()
{
	GLuint hdrFBO = 0;
	GLuint textureID = 0;
	GLuint hdrRBODepth = 0;
}



void HDRProcess::InitBloom(GLuint width, GLuint height)
{
	this->width = width;
	this->height = height;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//floating point color buffer
	glGenTextures(2, colorBufferIDs);
	for (size_t i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, colorBufferIDs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colorBufferIDs[i], 0);
	}
	
	//depth buffer
	glGenRenderbuffers(1, &hdrRBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	//attach buffers
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBODepth);
	attachments[0] = GL_COLOR_ATTACHMENT0;
	attachments[1] = GL_COLOR_ATTACHMENT1;

	glDrawBuffers(2, attachments);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	 
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Float Framebuffer ERROR:%d\n", status);
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/************* blur process ****************/

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorBIDS);
	for (size_t i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorBIDS[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorBIDS[i], 0);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			printf("Blur Framebuffer ERROR:%d\n", status);
			return;
		}
	}

}

void HDRProcess::WriteFloatBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
}

void HDRProcess::ReadFloatBuffer(GLenum textureUnit,GLuint colorBufferIndex)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, colorBufferIDs[colorBufferIndex]);
}

void HDRProcess::WriteColorBuffer(GLuint colorIndex)
{
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[colorIndex]);
}

void HDRProcess::ReadColorBuffer(GLenum textureUnit, GLuint colorBufferIndex)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, pingpongColorBIDS[colorBufferIndex]);
}


