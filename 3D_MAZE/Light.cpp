#include "Light.h"

Light::Light()
{
	xLightPos = 0;
	yLightPos = 0;
	zLightPos = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LightVertices) , LightVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(LightIndices) , LightIndices, GL_STATIC_DRAW);
	//binding GL_ARRAY_BUFFER to VAO_1
		//vertices coord
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Light::Light(GLfloat xLightPos, GLfloat yLightPos, GLfloat zLightPos):Light()
{
	this->xLightPos = xLightPos;
	this->yLightPos = yLightPos;
}

void Light::Translate(GLfloat transX, GLfloat transY, GLfloat transZ)
{
	TransModel = glm::translate(TransModel, glm::vec3(transX, transY, transZ));
}

void Light::Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	ScaleModel = glm::scale(ScaleModel, glm::vec3(scaleX, scaleY, scaleZ));
}

void Light::Draw(GLuint uniformLocation)
{
	TransModel = glm::mat4(1.0f);
	Translate(xLightPos, yLightPos, zLightPos);
	ScaleModel = glm::mat4(1.0f);
	Scale(0.05f, 0.05f, 0.05f);
	SetModel(uniformLocation);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Light::~Light()
{
}

void Light::SetModel(GLuint uniformLocation)
{
	glm::mat4 modelMatrix = TransModel * ScaleModel;
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
