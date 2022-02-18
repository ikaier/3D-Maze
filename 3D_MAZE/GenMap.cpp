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
	WallLightCount = 0;
	map = std::vector<std::vector<struct grid>>(xNum, std::vector<struct grid>(yNum));
	for (size_t i = 0; i < xNum; i++) {
		for (size_t j = 0; j < yNum; j++) {
			grid newgrid = { true, true, false };
			map[i][j] = newgrid;
		}
	}
	recBackTrack(3, 2);
	printMap();
	static const char* vLightingShader = "Shaders/Lightingshader.vert";
	static const char* fLightingShader = "Shaders/Lightingshader.frag";
	Lightingshader = new Shader();
	Lightingshader->CreateFromFiles(vLightingShader, fLightingShader);
	anFloor =new Floor(xNum, yNum, gridSize);
	CreateWalls(); 
	CreateWallLights();

	
	//SetupMap();
}

void GenMap::Draw(Shader& shader,glm::mat4 projection,glm::mat4 view)
{
	shader.setFloat("material.shininess", FloorShiness);
	shader.setFloat("material.specularIntensity", FloorSpecularIntensity);

	for (size_t i = 0; i < WallLightCount; i++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		shader.setFloat(locBuff, ambientIntensity);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		shader.setFloat(locBuff, diffuseIntensity);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.constant", i);
		shader.setFloat(locBuff, constant);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.linear", i);
		shader.setFloat(locBuff, linear);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.exponent", i);
		shader.setFloat(locBuff, exponent);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		shader.setvec3(locBuff, glm::vec3(red, green, blue));
		
	}
	shader.SetPointLights(wallLightList, WallLightCount);
	
	
	anFloor->Draw(shader.GetTransformLocation());

	shader.setFloat("material.shininess", WallShiness);
	shader.setFloat("material.specularIntensity", WallSpecularIntensity);

	anWall->Draw(shader.GetTransformLocation());
	DrawLightCubes( projection, view);
}

void GenMap::DrawLightCubes(glm::mat4 projection, glm::mat4 view)
{
	Lightingshader->UseShader();
	Lightingshader->setMat4("projection", projection);
	Lightingshader->setMat4("view", view);
	Lightingshader->setvec4("Color", glm::vec4(red + (1 - red) * 0.3f, green + (1 - green) * 0.3f, blue + (1 - blue) * 0.3f, 0.8f));
	for (size_t i = 0; i < WallLightCount; i++) {
		wallLightList[i].DrawCubes(Lightingshader->GetTransformLocation());
	}
}

GenMap::~GenMap()
{
	//delete anFloor;
	//delete anWall;
}

void GenMap::recBackTrack(GLint x, GLint y)
{
	assert(x < xNum&& y < yNum);

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

void GenMap::CreateWallLights()
{
	GLuint lightnumber = xNum * yNum / 4;
	if (lightnumber > MAX_POINT_LIGHTS) lightnumber = MAX_POINT_LIGHTS;

	for (int x = 0; x < xNum; x++) {
		for (int y = 0; y < yNum; y++) {
			int d=rand() % 100;
			if (d < 25) {
				if (map[x][y].bot) {
					AddWallLights(x * gridSize+gridSize / 2, 0.2f, -y * gridSize-0.01f);
				}
				else if (map[x][y].right) {
					AddWallLights((x+1) * gridSize -0.06f, 0.2f, -y * gridSize- gridSize / 2);
				}
			}
		}
	}
	//printf("%d",lightnumber);
	//AddWallLights(0.3f, 0.3f, 0.0f);
	//AddWallLights(1.0f, 0.3f, -1.0f);
}

void GenMap::AddWalls(GLfloat xPos, GLfloat yPos,bool ifRotate)
{
	xWallPos.push_back(xPos);
	yWallPos.push_back(yPos);
	ifRotates.push_back(ifRotate);
	WallCount++;

}

void GenMap::AddWallLights(GLfloat xPos, GLfloat yPos, GLfloat zPos)
{
	assert(WallLightCount < MAX_POINT_LIGHTS);
	WallLight aLight = WallLight(xPos, yPos, zPos);
	//printf("%f,%f,%f\n", xPos, yPos, zPos);
	wallLightList[WallLightCount] = aLight;
	WallLightCount++;
}


