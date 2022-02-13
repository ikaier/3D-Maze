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
		modelMatrics = new glm::mat4[1];
		SetModel(0);
		SendModel(uniformLocation, 1);
	}
	else {
		count = 0;
		modelMatrics = new glm::mat4[WallCount];
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
			
			SetModel(count);
			count++;
		}
		SendModel(uniformLocation, count);
		
	}
}

void Wall::SetTexture()
{
	texture->UseTexture();
}

void Wall ::SetModel(GLuint uniformLocation)
{
	glm::mat4 modelMatrix = TransModel * RotateModel * ScaleModel;
	modelMatrics[count] = modelMatrix;
	//glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}
void Wall::SendModel(GLuint uniformLocation, GLuint count)
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrics[0], GL_STATIC_DRAW);

	unsigned int VAO = obj->GetVAO();
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
	obj->RenderMesh(count);
	
}
Wall::~Wall()
{
    
}
