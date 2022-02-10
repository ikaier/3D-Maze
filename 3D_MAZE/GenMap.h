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

class GenMap
{
public:
	GenMap();
	GenMap(GLuint width, GLuint height, GLfloat gridSize);
	void Draw(GLuint uniformLocation);
	~GenMap();
private:
	GLuint xNum, yNum;
	GLint xEnd=-1, yEnd=-1;
	GLfloat gridSize;
	GLuint WallCount;
	Floor* anFloor;
	Wall* anWall;
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
	void AddWalls(GLfloat xPos,GLfloat yPos, bool ifRotate);
	
};

