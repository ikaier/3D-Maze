#include "Floor.h"

Floor::Floor()
{
	
	
	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	
	xNum = 0;
	yNum = 0;
	
}

Floor::Floor(GLuint xNum, GLuint yNum,GLfloat gridSize):Floor()
{
	obj = Mesh();

	obj.CreateMesh(Floorvertices, Floorindices, 32, 6);
	texture = new Texture("Textures/floor.bmp");

	texture->LoadTexture();
	this->xNum = xNum;
	this->yNum = yNum;
	this->gridSize = gridSize;
	Scale(gridSize, 1.0f, gridSize);
	Set();
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

void Floor::Set()
{

	//single floor grid
	if (xNum == 0 && yNum == 0) {
		modelMatrics.clear();
		//modelMatrics = new glm::mat4[1];
		SetModel();
		SendModel();
		

	}
	else {
		//map
		count = 0;
		//modelMatrics = new glm::mat4[xNum * yNum];
		modelMatrics.clear();
		for (GLfloat x = 0.0f; x < xNum; x = x + 1.0f) {
			for (GLfloat y = 0.0f; y < yNum; y = y + 1.0f) {
				TransModel = glm::mat4(1.0f);
				Translate(x * gridSize, 0, -y * gridSize);
				SetModel();
				count++;
				//
			}
		}
		
		SendModel();
	}
}

void Floor::Draw()
{
	SetTexture();
	obj.RenderMesh(count);
}


void Floor::SetTexture()
{

	texture->UseTexture();
}

void Floor::SetModel()
{
	glm::mat4 RotatMatix = glm::toMat4(quaternion);
	glm::mat4 modelMatrix = TransModel * RotatMatix * ScaleModel;
	modelMatrics.push_back( modelMatrix);
	//glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Floor::SendModel()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrics[0], GL_STATIC_DRAW);

	unsigned int VAO = obj.GetVAO();
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


Floor::~Floor()
{
	
}
