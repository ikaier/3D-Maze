#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>

#include<GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition,glm::vec3 cameraY,
		GLfloat startYaw, GLfloat startPitch, GLfloat startRoll,
		GLfloat startMoveSpeed,GLfloat startTurnSpeed);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::vec4 getViewMatrix();

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void flip(GLfloat deltaTime);

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 CameraZ;
	glm::vec3 CameraY;
	glm::vec3 CameraX;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();

};

