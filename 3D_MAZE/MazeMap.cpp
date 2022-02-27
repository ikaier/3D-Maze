#include "MazeMap.h"

MazeMap::MazeMap()
{
	xNum = 0;
	yNum = 0;
	WallCount = 0;
	WallLightCount = 0;
	PolyCount = 0;
	gridSize = 0;
	won = false;
}

MazeMap::MazeMap(GLuint width, GLuint height, GLfloat gridSize):MazeMap()
{
	srand(time(0));
	assert(width > 0 && height > 0);
	xNum = width;
	yNum = height;
	this->gridSize = gridSize;
	map = std::vector<std::vector<struct grid>>(xNum, std::vector<struct grid>(yNum));
	for (size_t i = 0; i < xNum; i++) {
		for (size_t j = 0; j < yNum; j++) {
			grid newgrid = { true, true, false,-1 };
			map[i][j] = newgrid;
		}
	}
	recBackTrack(3, 2);
	printMap();
	CreateWallLights();
	CreatePolys();
}

glm::vec3 MazeMap::CollionDetection(glm::vec3 position)
{

	GLuint xCurrent = std::floor(abs(position.x) / gridSize);
	GLuint yCurrent = std::floor(abs(position.z) / gridSize);
	GLfloat xGridPos = fmod((fmod(abs(position.x), gridSize) + gridSize), gridSize);
	GLfloat yGridPos = fmod((fmod(abs(position.z), gridSize) + gridSize), gridSize);
	glm::vec3 output = glm::vec3(0.0f,0.0f,0);
	//printf("xcurrent= %d, ycurrent= %d, yNum=%d, xgrid= %f, ygrid= %f\n", xCurrent, yCurrent, yNum, xGridPos, yGridPos);
	if (xGridPos < 0.14) {
		if (xCurrent == 0 || map[xCurrent - 1][yCurrent].right)output.x += (0.14f- xGridPos);
	}
	else if (xGridPos > 0.66) {
		if (map[xCurrent][yCurrent].right)output.x -= (xGridPos-0.66f);
	}
	if (yGridPos < 0.16) {
		if (map[xCurrent][yCurrent].bot)output.y -= (0.16f-yGridPos);
	}
	else if (yGridPos > 0.66) {
		if (yCurrent == yNum-1||map[xCurrent][yCurrent+1].bot)output.y += ( yGridPos-0.66f);
	}
	if (map[xCurrent][yCurrent].poly!=-1) {
		printf("Counter Icosahedron at (%d, %d)\n", xCurrent, yCurrent);
		polys[map[xCurrent][yCurrent].poly] = glm::vec3(-1, -1, -1);
		PolyCount--;
		map[xCurrent][yCurrent].poly = -1;
		output.z = 1;
	}
	if (xCurrent == xEnd && yCurrent == yEnd && !won) {
		printf("You Won! time spend: %f seconds\nPlease press ESC to quit", glfwGetTime());
		won = true;
	};
	return output;
}

void MazeMap::recBackTrack(GLint x, GLint y)
{
	assert(x < xNum&& y < yNum);

	map[x][y].visited = true;
	direction arraydirecs[4]{ left,right,up,down };
	GLint tx, ty;
	shuffleArray(arraydirecs, 4);

	bool end = true;
	for (int i = 0; i < 4; i++)
	{
		tx = x + moveX[arraydirecs[i]];
		ty = y + moveY[arraydirecs[i]];
		assert(abs(x - tx) + abs(y - ty) == 1);
		if (tx >= xNum || ty >= yNum || tx < 0 || ty < 0 || map[tx][ty].visited == true)
		{
			continue;
		}
		else
		{
			end = false;
			if (x - tx == 1 && y - ty == 0)
			{
				map[tx][ty].right = false;
			}
			else if (x - tx == -1 && y - ty == 0) {
				map[x][y].right = false;
			}
			else if (x - tx == 0 && y - ty == -1) {
				map[tx][ty].bot = false;
			}
			else if (x - tx == 0 && y - ty == 1) {
				map[x][y].bot = false;
			}
			recBackTrack(tx, ty);
		}
	}
	if (end && xEnd == -1 && yEnd == -1) {
		xEnd = x;
		yEnd = y;
	}

}

void MazeMap::shuffleArray(direction* arr, int size)
{
	for (int i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(&arr[i], &arr[j]);
	}
}

void MazeMap::swap(direction* a, direction* b)
{
	direction temp = *a;
	*a = *b;
	*b = temp;
}

void MazeMap::printMap()
{
	//print top row
	for (size_t xi = 0; xi < xNum; xi++) {
		AddWalls(xi * gridSize, yNum * gridSize, false);
		printf("__");
	}
	printf("\n");
	for (int i = yNum - 1; i >= 0; i--)
	{
		//left border
		printf("|");

		AddWalls(0.0f, i * gridSize, true);
		for (int x = 0; x < xNum; x++) {
			//x direction
			if (map[x][i].bot) {
				printf("_");
				AddWalls(x * gridSize, i * gridSize, false);
			}
			else {
				printf(" ");
			}
			//y direction
			if (map[x][i].right) {
				printf("|");
				AddWalls((x + 1) * gridSize, i * gridSize, true);
			}
			else {
				printf(" ");
			}

		}
		printf("\n");
	}
	printf("The end is at (%d, %d)", xEnd + 1, yEnd + 1);
}

void MazeMap::CreateWallLights()
{
	GLuint lightnumber = xNum * yNum / 4;
	if (lightnumber > MAX_POINT_LIGHTS) lightnumber = MAX_POINT_LIGHTS;
	//Can be improved: randomly generate x,y instead of looping
	for (int x = 0; x < xNum; x++) {
		for (int y = 0; y < yNum; y++) {
			GLuint d = rand() % 100;
			if (WallLightCount <lightnumber && d < (GLuint)(100 * ((GLfloat)lightnumber /(xNum * yNum)))) {
				if (map[x][y].bot) {
					AddWallLights(x * gridSize + gridSize / 2, 0.2f, -y * gridSize - 0.01f);
				}
				else if (map[x][y].right) {
					AddWallLights((x + 1) * gridSize - 0.06f, 0.2f, -y * gridSize - gridSize / 2);
				}
			}
		}
	}
}

void MazeMap::CreatePolys()
{
	GLuint polyNumber = xNum * yNum / 50;
	if (polyNumber == 0) polyNumber = 1;
	int ind= 0;
	while (PolyCount < polyNumber) {
		GLint randomX = rand() % xNum;
		GLint randomY = rand() % yNum;
		map[randomX][randomY].poly = ind;
		ind++;
		AddPolys(randomX * gridSize + gridSize / 2, -randomY * gridSize - gridSize / 2);
	}

}

void MazeMap::AddWalls(GLfloat xPos, GLfloat yPos, bool ifRotate)
{
	walls.push_back(glm::vec3(xPos, yPos, ifRotate));
	WallCount++;
}

void MazeMap::AddWallLights(GLfloat xPos, GLfloat yPos, GLfloat zPos)
{
	assert(WallLightCount < MAX_POINT_LIGHTS);
	wallLights.push_back(glm::vec3(xPos, yPos, zPos));
	WallLightCount++;
}

void MazeMap::AddPolys(GLfloat xPos, GLfloat zPos)
{
	polys.push_back(glm::vec3(xPos, gridSize*3/10, zPos));
	PolyCount++;
}


