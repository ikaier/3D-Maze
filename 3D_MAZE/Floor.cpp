#include "Floor.h"

Floor::Floor()
{
	
	obj = new Mesh();
	
	obj->CreateMesh(Floorvertices, Floorindices, 32, 6);

	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	texture = new Texture("Textures/floor.bmp");

	texture->LoadTexture();
	xNum = 0;
	yNum = 0;
	
}

Floor::Floor(GLuint xNum, GLuint yNum):Floor()
{
	this->xNum = xNum;
	this->yNum = yNum;
	floorPositions= std::vector<glm::vec3>(xNum * yNum);
	
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

void Floor::Draw(GLuint uniformLocation)
{
	SetTexture();
	//single floor grid
	if (xNum == 0 && yNum == 0) {
		SetModel(uniformLocation);
		obj->RenderMesh();
	}
	else {
		//map
		for (GLfloat x = 0.0f; x < xNum; x = x + 1.0f) {
			for (GLfloat y = 0.0f; y < yNum; y = y + 1.0f) {
				TransModel = glm::mat4(1.0f);
				Translate(x * 0.25, 0, -y * 0.25f);
				SetModel(uniformLocation);
				obj->RenderMesh();
			}
		}
	}
}


void Floor::SetTexture()
{
	texture->UseTexture();
}

void Floor::SetModel(GLuint uniformLocation)
{
	glm::mat4 RotatMatix = glm::toMat4(quaternion);
	glm::mat4 modelMatrix = TransModel * RotatMatix * ScaleModel;
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}


Floor::~Floor()
{
	delete texture;
}
