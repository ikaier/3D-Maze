#pragma once
#include<stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<assert.h>
#include<algorithm>
#include<random>
#include<ctime>
#include <assert.h>

#include"Floor.h"
#include"Wall.h"
#include"Shader.h"

class GenMap
{
public:
	GenMap();
	GenMap(GLuint width, GLuint height, GLfloat gridSize);
	void Draw(Shader& shader, glm::mat4 projection, glm::mat4 view);
	void DrawLightCubes(glm::mat4 projection, glm::mat4 view);
	~GenMap();
private:
	//Wall property
	 GLfloat WallSpecularIntensity = 0.8f;
	 GLfloat WallShiness = 256;

	//wall light property
	 GLfloat red = 246.0f / 255.0f;
	 GLfloat green = 228.0f / 255.0f;
	 GLfloat blue = 188.0f / 255.0f;
	 GLfloat ambientIntensity = 0.2f;
	 GLfloat diffuseIntensity = 0.3f;
	 GLfloat constant = 1.0f;
	 GLfloat linear = 0.35f;
	 GLfloat exponent = 1.8f;

	 WallLight wallLightList[MAX_POINT_LIGHTS];

	GLuint xNum, yNum;
	GLint xEnd=-1, yEnd=-1;
	GLfloat gridSize;
	GLuint WallCount;
	GLuint WallLightCount;
	Floor* anFloor;
	Wall* anWall;
	Shader* Lightingshader;
	std::vector<GLfloat> xWallPos;
	std::vector<GLfloat> yWallPos;
	std::vector<bool> ifRotates;
	typedef struct grid
	{
		bool bot;
		bool right;
		bool visited;
	};
	GLint moveX[4] = { -1,1,0,0 };
	GLint moveY[4] = { 0,0,1,-1 };
	std::vector<std::vector<struct grid>> map;
	enum direction { left, right, up, down };
	void recBackTrack(GLint x, GLint y);
	void shuffleArray(direction* arr, int size);
	void swap(direction *a,direction *b);
	void printMap();
	void CreateWalls();
	void CreateWallLights();
	void AddWalls(GLfloat xPos,GLfloat yPos, bool ifRotate);
	void AddWallLights(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	
};

