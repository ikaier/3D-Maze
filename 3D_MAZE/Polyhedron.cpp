#include "Polyhedron.h"

Polyhedron::Polyhedron()
{

	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	RotateModel = glm::mat4(1.0f);
	polysCount = 0;
    VAO = 0;
    VBO = 0;
	VNO = 0;
	gridSize = 0;
}

Polyhedron::Polyhedron(std::vector<glm::vec3> polys, GLuint polysCount, GLfloat gridSize):Polyhedron()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &VNO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(polyVertices), polyVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VNO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormal), vertexNormal, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->polysCount = polysCount;
	this->polys = polys;
	this->gridSize = gridSize;
	
}

void Polyhedron::Rotate(float Angle)
{
	RotateModel = glm::mat4(1.0f);
    RotateModel = glm::rotate(RotateModel, glm::radians(Angle), glm::vec3(0.0f, 1.0f, 0.0f));
	
}

void Polyhedron::Translate(GLfloat transX, GLfloat transY, GLfloat transZ)
{
	TransModel = glm::mat4(1.0f);
    TransModel = glm::translate(TransModel, glm::vec3(transX, transY, transZ));
}

void Polyhedron::Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	ScaleModel = glm::mat4(1.0f);
    ScaleModel = glm::scale(ScaleModel, glm::vec3(scaleX, scaleY, scaleZ));
}

void Polyhedron::Draw()
{
	
	glBindVertexArray(VAO);
	//glDrawElementsInstanced(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0, polysCount);
	glDrawArraysInstanced(GL_TRIANGLES, 0,60, polysCount);
	glBindVertexArray(0);
}

void Polyhedron::Set()
{
	modelMatrics.clear();
	for (size_t i = 0; i < polysCount; i++) {
		TransModel = glm::translate(glm::mat4(1.0f), glm::vec3(polys[i].x, polys[i].y, polys[i].z));
		ScaleModel= glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		SetModel();
	}
	SendModel();

}

Polyhedron::~Polyhedron()
{
}


void Polyhedron::SetModel()
{
	glm::mat4 modelMatrix = TransModel * RotateModel* ScaleModel;
    modelMatrics.push_back(modelMatrix);
}

void Polyhedron::SendModel()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, polysCount * sizeof(glm::mat4), &modelMatrics[0], GL_STATIC_DRAW);


	glBindVertexArray(VAO);
	// vertex attributes
	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}
