#include "Floor.h"

Floor::Floor()
{
	obj = new Mesh();
	obj->CreateMesh(Floorvertices, Floorindices, 32, 6);
	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
}


void Floor::Rotate(float RotateX, float RotateY, float RotateZ)
{
	glm::vec3 EulerAngles(RotateX, RotateY, RotateZ);
	glm::quat temp_quat = glm::quat(EulerAngles);
	quaternion = temp_quat * quaternion;
}

void Floor::Translate(GLfloat transX, GLfloat transY, GLfloat transZ)
{
	TransModel = glm::translate(TransModel, glm::vec3(transX, transY, transZ));
}

void Floor::Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	ScaleModel = glm::scale(ScaleModel, glm::vec3(scaleX, scaleY, scaleZ));
}

void Floor::Render()
{
	obj->RenderMesh();
}

void Floor::SetModel(GLuint uniformLocation)
{
	glm::mat4 RotatMatix = glm::toMat4(quaternion);
	glm::mat4 modelMatrix = TransModel * RotatMatix * ScaleModel;
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}


Floor::~Floor()
{
}
