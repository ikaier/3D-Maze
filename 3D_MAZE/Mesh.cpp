#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexNumber = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices,
	unsigned int numberOfVertices, unsigned int numberOfIndices)
{
	indexNumber = numberOfIndices;
	//bind Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

	//bind element array buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

	//binding GL_ARRAY_BUFFER to VAO_1
		//vertices coord
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
		//texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
		//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
//

void Mesh::RenderMesh(GLuint count)
{
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indexNumber, GL_UNSIGNED_INT, 0, count);
	//glDrawElements(GL_TRIANGLES, indexNumber, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint Mesh::GetVAO()
{
	return VAO;
}

void Mesh::ClearMesh()
{
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexNumber = 0;
}

Mesh::~Mesh()
{
	//ClearMesh();
}
