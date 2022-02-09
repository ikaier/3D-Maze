#pragma once
#include<stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<assert.h>
#include<algorithm>
#include<random>
#include<ctime>

#include"Floor.h"
class GenMap
{
public:
	GenMap();
	GenMap(GLuint width, GLuint height, GLuint gridSize);
	~GenMap();
private:
	GLuint xNum, yNum;
	GLint xEnd=-1, yEnd=-1;
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
	
};

