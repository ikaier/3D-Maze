#include "Wall.h"

Wall::Wall()
{
	obj = new Mesh();

	obj->CreateMesh(WallVertices, WallIndices, 192, 36);

	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	texture = new Texture("Textures/wall.bmp");

	texture->LoadTexture();

}
void Wall::Rotate(float RotateX, float RotateY, float RotateZ)
{
	glm::vec3 EulerAngles(RotateX, RotateY, RotateZ);
	glm::quat temp_quat = glm::quat(EulerAngles);
	quaternion = temp_quat * quaternion;
}

void Wall::Translate(GLfloat transX, GLfloat transY, GLfloat transZ)
{
	TransModel = glm::translate(TransModel, glm::vec3(transX, transY, transZ));
}

void Wall::Scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	ScaleModel = glm::scale(ScaleModel, glm::vec3(scaleX, scaleY, scaleZ));
}

void Wall::Draw(GLuint uniformLocation)
{
	SetTexture();
	SetModel(uniformLocation);
	obj->RenderMesh();
}

void Wall::SetTexture()
{
	texture->UseTexture();
}

void Wall ::SetModel(GLuint uniformLocation)
{
	glm::mat4 RotatMatix = glm::toMat4(quaternion);
	glm::mat4 modelMatrix = TransModel * RotatMatix * ScaleModel;
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
Wall::~Wall()
{
    
}
