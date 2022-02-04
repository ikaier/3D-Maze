#include "MyTransform.h"


MyTransform::MyTransform()
{
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
}

MyTransform::MyTransform(GLfloat transX, GLfloat transY, GLfloat transZ, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ, GLfloat RotateX, GLfloat RotateY, GLfloat RotateZ)
{
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(RotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(RotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(RotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(transX,transY,transZ));
	model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
	//glm::quat model;
	//glm::vec3 EulerAngles(transX, transY, transZ);
	//model = glm::quat(EulerAngles);


}

void MyTransform::ApplyTransform(GLuint uniformLocation)
{
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(model));
}

MyTransform::~MyTransform()
{
}
