#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 cameraY, 
	GLfloat startYaw, GLfloat startPitch, GLfloat startRoll, 
	GLfloat startMoveSpeed, GLfloat startTurnSpeed,
	GLuint xNum, GLuint yNum, GLfloat gridSize):xNum(xNum),yNum(yNum),gridSize(gridSize)
{
	position = startPosition;
	worldUp = cameraY;
	yaw = startYaw;
	pitch = startPitch;
	roll = startRoll;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	quaternion = glm::quat(glm::vec3(0.0, 0.0, 0.0));
	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	//flash light
	godMode = false;
	update();
}


glm::vec3 Camera::getCameraPosition()
{

	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position,position+front,up);
}


void Camera::keyControl(GLfloat* keys)
{
	position += front * keys[0];
	position -= front * keys[1];
	position -= right * keys[2];
	position += right * keys[3];
	if (!godMode)
	{
		position.y = gridSize * 3 / 10;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{

	yaw += xChange;
	pitch += yChange;

	//limit pitching angle
	if (pitch > 1.4f) {
		pitch = 1.4f;
	}
	if (pitch < -1.4f) {
		pitch = -1.4f;
	}

	
	update();
}

void Camera::flip()
{

	roll += glm::radians(180.0f);
	update();
}

Camera::~Camera()
{
}

void Camera::CollionRes(glm::vec3 adjust)
{
	
	position.x += adjust.x;

	position.z += adjust.y;
	if (adjust.z) {
		flip(); 
	};
	update();

}

void Camera::GodModeToggle()
{
	if (godMode) {
		//turn off god mode
		GoalPosition = position;

		if (position.x <=0) { GoalPosition.x = gridSize / 2; }
		else if (position.x >= (float)(xNum-1) * gridSize) { GoalPosition.x = ((float)(xNum - 0.5)) * gridSize; }
		GoalPosition.y = 0.2f;
		if (position.z >= 0) { GoalPosition.z = -gridSize / 2; }
		else if (position.z <= -(float)(yNum-1) * gridSize) { GoalPosition.z = -((float)(xNum - 0.5)) * gridSize; }
		printf("Back to %f, %f, %f\n", GoalPosition.x, GoalPosition.y, GoalPosition.z);
		position = GoalPosition;
	}

	godMode = !godMode;
}

void Camera::update()
{
	//glm::quat qRoll = glm::angleAxis(-pitch, glm::vec3(0, 0, 1));
	//glm::quat qPitch = glm::angleAxis(-pitch, glm::vec3(1, 0, 0));
	//glm::quat qYaw = glm::angleAxis(-yaw, glm::vec3(0, 1, 0));
	//quaternion = qYaw *glm::quat(glm::vec3(0.0f,0.0f,0.0f)) * qPitch;
	glm::quat quaternion = glm::quat(glm::vec3(-pitch, -yaw, -roll));


	front = glm::vec3(
		-2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),
		-2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		-1.0f + 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y)
	);

	up = glm::vec3(
		2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z),
		2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x)
	);

	right = glm::vec3(
		(1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z)),
		(2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z)),
		(2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y))
	);

	
}


