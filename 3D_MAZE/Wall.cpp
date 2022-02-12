#include "Wall.h"

Wall::Wall()
{
	WallCount = 0;
	obj = new Mesh();

	obj->CreateMesh(WallVertices, WallIndices, 192, 36);

	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	RotateModel = glm::mat4(1.0f);

	texture = new Texture("Textures/wall.bmp");
	gridSize = 0;
	texture->LoadTexture();

}
Wall::Wall(std::vector<GLfloat> xWallPos, std::vector<GLfloat> yWallPos, std::vector<bool> ifRotates, GLuint WallCount,GLfloat gridSize):Wall()
{
	this->WallCount = WallCount;
	this->xWallPos = xWallPos;
	this->yWallPos = yWallPos;
	this->gridSize = gridSize;
	this->ifRotates = ifRotates;
	Scale(gridSize, gridSize*3/5, 1.0f);
	/*for (size_t i = 0; i < xNum; i++) {
		this->xWallPos.push_back( xWallPos[i]);
	}
	for (size_t i = 0; i < xNum; i++) {
		this->yWallPos.push_back(yWallPos[i]);
	}*/
}
void Wall::Rotate(float yAngle)
{
	RotateModel = glm::rotate(RotateModel, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
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
	
	//printf(" %f ", gridSize);
	assert(gridSize > 0);
	SetTexture();
	if (WallCount == 0) {
		SetModel(uniformLocation);
		obj->RenderMesh();
	}
	else {
		for (size_t i = 0; i < WallCount; i = i + 1) {
			//printf("%f,%f\n", xWallPos[i], yWallPos[i]);	
			TransModel = glm::mat4(1.0f);	
			if (ifRotates[i]) {
				Translate(xWallPos[i], 0, -yWallPos[i]);
				RotateModel = glm::mat4(1.0f);
				Rotate(90.0);	
			}
			else {
				Translate(xWallPos[i], 0, -yWallPos[i]);
				RotateModel = glm::mat4(1.0f);
			}
			SetModel(uniformLocation);
			obj->RenderMesh();	
			
		}
		
	}
}

void Wall::SetTexture()
{
	texture->UseTexture();
}

void Wall ::SetModel(GLuint uniformLocation)
{
	glm::mat4 modelMatrix = TransModel * RotateModel * ScaleModel;
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
Wall::~Wall()
{
    
}
