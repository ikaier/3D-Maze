#pragma once
#include<stdio.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>
#include<assert.h>
#include<algorithm>
#include<random>
#include<ctime>
#include"CommonValues.h"
class MazeMap
{
public:
	MazeMap();
	MazeMap(GLuint width, GLuint height, GLfloat gridSize);
	GLuint GetWallCount() { return WallCount; };
	GLuint GetWallLightCount() { return WallLightCount; };
	GLuint GetPolyCount() { return PolyCount; };
	std::vector<glm::vec3> GetWalls() { return walls; };
	std::vector<glm::vec3> GetWallLights() { return wallLights; };
	std::vector<glm::vec3> GetPolys() { return polys; };
	glm::vec2 CollionDetection(glm::vec3 position);
private:
	GLuint xNum, yNum;
	GLint xEnd = -1, yEnd = -1;
	GLfloat gridSize;

	GLuint WallCount;
	GLuint WallLightCount;
	GLuint PolyCount;

	std::vector<glm::vec3> walls;
	std::vector<glm::vec3> wallLights;
	std::vector<glm::vec3> polys;

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
	void swap(direction* a, direction* b);
	void printMap();
	void CreateWallLights();
	void CreatePolys();
	void AddWalls(GLfloat xPos, GLfloat yPos, bool ifRotate);
	void AddWallLights(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	void AddPolys(GLfloat xPos, GLfloat zPos);

};

