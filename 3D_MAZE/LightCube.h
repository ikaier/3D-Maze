#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"Mesh.h"

class LightCube
{
public:
	LightCube();
	LightCube(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos,
		GLfloat red, GLfloat green,GLfloat blue);

	void Translate(GLfloat transX, GLfloat transY, GLfloat transZ);
	void Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	void Draw(GLuint uniformLocation);
	~LightCube();

private:
	GLfloat xLightPos, yLightPos, zLightPos, red, green, blue;
	GLuint VAO, VBO, EBO;
	unsigned int LightIndices[36] = {
		1,0,3,
		3,2,1,

		5,4,0,
		0,1,5,

		2,3,7,
		7,6,2,

		4,5,6,
		6,7,4,
		
		0,4,7,
		7,3,0,

		5,1,2,
		2,6,5
	};
	GLfloat LightVertices[24] = {
		1.0f,	1.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,

		1.0f,	1.0f,	-1.0f,
		1.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,
		0.0f,	1.0f,	-1.0f
	};
	glm::mat4 TransModel;
	glm::mat4 ScaleModel;
	void SetModel(GLuint uniformLocation);
};

