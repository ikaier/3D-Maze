#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<GL/glew.h>
#include"MyTransform.h"
class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	
	void Validate();
	void UseShader();
	void ClearShader();

	GLuint GetTransformLocation();
	GLuint GetViewLocation();
	GLuint GetProjectionLocation();

private:
	GLuint shaderID, transformLocation, viewLocation, projectionLocation;
	std::string ReadFile(const char* fileLocation);
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	GLuint AttachShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileProgram();
};

