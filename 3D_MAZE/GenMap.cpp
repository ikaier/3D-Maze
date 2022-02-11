#include "GenMap.h"

GenMap::GenMap()
{
	xNum = 0;
	yNum = 0;
	WallCount = 0;
	gridSize = 0;
}

GenMap::GenMap(GLuint width, GLuint height, GLfloat gridSize)
{
	srand(time(0));
	assert(width > 0 && height > 0);
	xNum = width;
	yNum = height;
	this->gridSize = gridSize;
	WallCount = 0;
	map = std::vector<std::vector<struct grid>>(xNum, std::vector<struct grid>(yNum));
	for (size_t i = 0; i < xNum; i++) {
		for (size_t j = 0; j < yNum; j++) {
			grid newgrid = { true, true, false };
			map[i][j] = newgrid;
		}
	}
	recBackTrack(3, 2);
	printMap();
	
	anFloor =new Floor(xNum, yNum, gridSize);
	CreateWalls();
	//SetupMap();
}

void GenMap::Draw(Shader& shader)
{
	
	anFloor->Draw(shader.GetTransformLocation());
	anWall->Draw(shader.GetTransformLocation());
}

GenMap::~GenMap()
{
	//delete anFloor;
	//delete anWall;
}

void GenMap::recBackTrack(GLint x, GLint y)
{
	map[x][y].visited = true;
	direction arraydirecs[4]{ left,right,up,down };
	GLint tx, ty;
	shuffleArray(arraydirecs, 4);
	//for (int i = 4 - 1; i >= 0; i--) {
	//	printf("%d", arraydirecs[i]);
	//}
	//printf("\n");
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
	if (end && xEnd == -1 && yEnd == -1){
		xEnd = x;
		yEnd = y;
	}

	
}

void GenMap::shuffleArray(direction* arr, int size)
{
	
	for (int i = size - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(&arr[i], &arr[j]);
	}
}

void GenMap::swap(direction* a, direction* b)
{
	direction temp = *a;
	*a = *b;
	*b = temp;
}

void GenMap::printMap()
{
	//print top row
	for (size_t xi = 0; xi < xNum; xi++) {
		AddWalls(xi * gridSize, yNum * gridSize,false);
		printf("__");
	}
	printf("\n");
	for (int i = yNum-1; i >= 0; i--)
	{
		//left border
		printf("|");

		//+-0.25 is a small trick to indicate that the wall need to be rotated
		AddWalls(0.0f, i * gridSize,true);
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
				AddWalls((x+1) * gridSize, i * gridSize, true);
			}
			else {
				printf(" ");
			}
			
			//printf("%s", (map[x][i].right ? "|" : " "));
			
		}
		printf("\n");
	}
	printf("The end is at (%d, %d)", xEnd+1, yEnd+1);
}

void GenMap::CreateWalls()
{
	assert(WallCount > 0 && gridSize > 0);
	anWall = new Wall(xWallPos, yWallPos,ifRotates, WallCount, gridSize);
	
}

void GenMap::AddWalls(GLfloat xPos, GLfloat yPos,bool ifRotate)
{
	xWallPos.push_back(xPos);
	yWallPos.push_back(yPos);
	ifRotates.push_back(ifRotate);
	WallCount++;

}


