#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtx/string_cast.hpp>
#include<GLFW/glfw3.h>
#include<iostream>
#include <cmath>

#include"FlashLight.h"
#include"Shader.h"
#include"MazeMap.h"

class Camera 
{
public:
	Camera();
	Camera(glm::vec3 startPosition,glm::vec3 cameraY,
		GLfloat startYaw, GLfloat startPitch, GLfloat startRoll,
		GLfloat startMoveSpeed,GLfloat startTurnSpeed,
		GLuint xNum,GLuint yNum,GLfloat gridSize);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 getViewMatrix();



	glm::vec3 NextPos(GLfloat* keys);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void flip(GLfloat deltaTime);

	void CollionRes(glm::vec3 adjust);

	~Camera();

private:

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::quat quaternion;

	GLuint xNum;
	GLuint yNum;
	GLfloat gridSize;
	bool godMode;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	GLfloat moveSpeed;
	GLfloat turnSpeed;



	
	void update();


};

