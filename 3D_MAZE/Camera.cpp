#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 cameraY, 
	GLfloat startYaw, GLfloat startPitch, GLfloat startRoll, 
	GLfloat startMoveSpeed, GLfloat startTurnSpeed)
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

void Camera::flip(GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	for (GLfloat i = 0; i <= 180.0f/ velocity; i++) {
		roll += i* velocity;
	}
	if (roll >= 360) {
		roll = 0;
	}
}

Camera::~Camera()
{
}

void Camera::update()
{

	glm::quat qPitch = glm::angleAxis(-pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(-yaw, glm::vec3(0, 1, 0));
	quaternion = qYaw *glm::quat(glm::vec3(0.0f,0.0f,0.0f)) * qPitch;

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


