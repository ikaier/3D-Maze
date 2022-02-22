#include "Wall.h"

Wall::Wall()
{
	

	TransModel = glm::mat4(1.0f);
	ScaleModel = glm::mat4(1.0f);
	RotateModel = glm::mat4(1.0f);
	WallCount = 0;
	
	gridSize = 0;
	

}
Wall::Wall(std::vector<glm::vec3> walls, GLuint WallCount,GLfloat gridSize):Wall()
{
	obj = Mesh();
	obj.CreateMesh(WallVertices, WallIndices, 192, 36);
	texture = new Texture("Textures/wall.bmp");
	texture->LoadTexture();
	this->WallCount = WallCount;
	this->walls = walls;
	this->gridSize = gridSize;

	Scale(gridSize, gridSize*3/5, 1.0f);
	Set();
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

void Wall::Set()
{
	modelMatrics.clear();
	//printf(" %f ", gridSize);
	assert(gridSize > 0);
	SetTexture();
	if (WallCount == 0) {
		SetModel();
		SendModel();
	}
	else {
		count = 0;
		for (size_t i = 0; i < WallCount; i = i + 1) {
			//printf("%f,%f\n", xWallPos[i], yWallPos[i]);	
			TransModel = glm::mat4(1.0f);	
			if (walls[i].z) {
				Translate(walls[i].x, 0, -walls[i].y);
				RotateModel = glm::mat4(1.0f);
				Rotate(90.0);	
			}
			else {
				Translate(walls[i].x, 0, -walls[i].y);
				RotateModel = glm::mat4(1.0f);
			}
			
			SetModel();
			count++;
		}
		SendModel();
		
	}
}

void Wall::SetTexture()
{
	texture->UseTexture();
}

void Wall::Draw()
{
	SetTexture();
	obj.RenderMesh(count);
}

void Wall ::SetModel()
{
	glm::mat4 modelMatrix = TransModel * RotateModel * ScaleModel;
	modelMatrics.push_back(modelMatrix);
	//glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
void Wall::SendModel()
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
Wall::~Wall()
{
	
}
