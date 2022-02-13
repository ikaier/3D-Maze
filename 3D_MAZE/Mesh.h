#pragma once
#include<GL/glew.h>
#include <iostream>
#include<stdio.h>
class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat* vertices, unsigned int* indices,
		unsigned int numberOfVertices, unsigned int numberOfIndices);
	void RenderMesh(GLuint count);
	GLuint GetVAO();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, EBO;
	GLsizei indexNumber;
};

