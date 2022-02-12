#pragma once
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<GL/glew.h>
#include"MyTransform.h"
#include"CommonValues.h"
#include"WallLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	
	void Validate();
	void UseShader();
	void ClearShader();

	void SetPointLights(WallLight* pLight, unsigned int lightCount);

	void setMat4(const std::string &name,const glm::mat4 &mat) const;
	void setvec4(const std::string& name, const glm::vec4& vec) const;
	void setvec3(const std::string& name, const glm::vec3& vec) const;
	void setFloat(const std::string& name, float value) const;
	GLuint GetTransformLocation();
	GLuint GetPositionLocation();


private:
	GLuint shaderID, transformLocation, viewLocation, projectionLocation, pointLightCountLocation;
	std::string ReadFile(const char* fileLocation);
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	GLuint AttachShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileProgram();


	struct {

		GLuint uniformPosition;

	}uniformPointLight[MAX_POINT_LIGHTS];
};

