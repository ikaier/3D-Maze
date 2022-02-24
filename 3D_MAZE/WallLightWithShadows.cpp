#include "WallLightWithShadows.h"

WallLightWithShadows::WallLightWithShadows()
{
}

WallLightWithShadows::WallLightWithShadows(std::vector<glm::vec3> wallLights, GLuint wallLightsCount, GLuint SetWLShadowNumber):
	wallLights(wallLights),wallLightsCount(wallLightsCount),SetWLShadowNumber(SetWLShadowNumber)
{
	if (wallLightsCount<SetWLShadowNumber)
	{
		SetWLShadowNumber = wallLightsCount;
	}
	for (size_t i = 0; i < SetWLShadowNumber; i++) {
		OmniShadowMap* temp = new OmniShadowMap();
		temp->Init(1024, 1024);
		wallLightsWithShadows.push_back(*temp);
	}
}

std::vector<glm::vec3> WallLightWithShadows::GetWLShadow(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	//std::vector<GLfloat> list;
	//for (size_t i = 0; i < wallLightsCount; i++) {
	//	list.push_back(squareDistance(wallLights[i]));
	//	//printf("%f ", squareDistance(wallLights[i]));
	//}
	//quickSelect(list);
	return quickSelect(wallLights);
}
//std::vector<glm::vec3> WallLightWithShadows::quickSelect(std::vector<GLfloat>& wallLights)
//{
//	GLuint left = 0;
//	GLuint right = wallLightsCount - 1;
//	GLuint pivotIndex = wallLightsCount;
//	while (pivotIndex != SetWLShadowNumber) {
//		printVector(wallLights);
//		pivotIndex = partition(wallLights, left, right);//problem here 
//		//printf("%d\n", pivotIndex);
//		if (pivotIndex < SetWLShadowNumber) {
//			left = pivotIndex;
//		}
//		else {
//			right = pivotIndex - 1;
//		}
//	}
//	return std::vector<glm::vec3>(wallLights.begin(), wallLights.begin() + SetWLShadowNumber);
//}
//
//GLuint WallLightWithShadows::partition(std::vector<GLfloat>& wallLights, GLuint left, GLuint right)
//{
//	GLuint pivot = choosePivot(left, right);
//	GLfloat pivotDistance = wallLights[pivot];
//	while (left < right) {
//		//printf("%d,%d\n", left,right);
//		if (wallLights[left] >= pivotDistance) {
//			std::swap(wallLights[left], wallLights[right]);
//
//			right--;
//		}
//		else {
//			left++;
//		}
//	}
//	if (wallLights[left] < pivotDistance) {
//		left++;
//	}
//	return left;
//}
//
//void WallLightWithShadows::printVector(std::vector<GLfloat> vec) {
//	for (size_t i = 0; i < wallLightsCount; i++) {
//		printf("%f ", vec[i]);
//	}
//	printf("\n");
//
//}

/*****************************************************************************************************/


std::vector<glm::vec3> WallLightWithShadows::quickSelect(std::vector<glm::vec3> &wallLights)
{
	GLuint left = 0;
	GLuint right = wallLightsCount - 1;
	GLuint pivotIndex = wallLightsCount;
	while (pivotIndex != SetWLShadowNumber) {
		pivotIndex = partition(wallLights, left, right);//problem here 
		//printf("%d\n", pivotIndex);
		if (pivotIndex < SetWLShadowNumber) {
			left = pivotIndex;
		}
		else {
			right = pivotIndex - 1;
		}
	}
	return wallLights;
}

GLuint WallLightWithShadows::partition(std::vector<glm::vec3> &wallLights, GLuint left, GLuint right)
{
	GLuint pivot = choosePivot(left, right);
	GLfloat pivotDistance = squareDistance(wallLights[pivot]);
	while (left < right) {
		//printf("%d,%d\n", left,right);
		if (squareDistance(wallLights[left]) >= pivotDistance) {
			std::swap(wallLights[left], wallLights[right]);

			right--;
		}
		else {
			left++;
		}
	}
	if (squareDistance(wallLights[left]) < pivotDistance) {
		left++;
	}
	return left;
}

GLuint WallLightWithShadows::choosePivot(GLuint left, GLuint right)
{
	return left + (right - left) / 2;
}

GLfloat WallLightWithShadows::squareDistance(glm::vec3 wallLight)
{
	return glm::exp2(wallLight.x-cameraPosition.x) + glm::exp2(wallLight.z - cameraPosition.z);
}
