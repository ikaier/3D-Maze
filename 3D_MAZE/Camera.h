#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtx/string_cast.hpp>
#include<GLFW/glfw3.h>
#include<iostream>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition,glm::vec3 cameraY,
		GLfloat startYaw, GLfloat startPitch, GLfloat startRoll,
		GLfloat startMoveSpeed,GLfloat startTurnSpeed);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 getViewMatrix();

	void keyControl(GLfloat* keys);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void flip(GLfloat deltaTime);

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::quat quaternion;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();

};

