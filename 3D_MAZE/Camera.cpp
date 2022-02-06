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

	update();

}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::vec3();
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
//angle limit
	
	//if (pitch + yChange > 89.0f) {
	//	yChange = 89.0f - pitch;
	//	pitch = 89.0f;
	//}else if (pitch + yChange < -89.0f) {
	//	yChange = -89.0f - pitch;
	//	pitch = -89.0f;
	//}
	//else {
	//	pitch += yChange;
	//}



	printf("xchange is %f \n", xChange);
	
	glm::vec3 EulerAngle(yChange, xChange, 0);
	glm::quat quatChange = glm::quat(EulerAngle);
	//glm::quat qPitch = glm::angleAxis(yChange, glm::vec3(1, 0, 0));
	//glm::quat qYaw = glm::angleAxis(xChange, glm::vec3(0, 1, 0));
	//glm::quat quatChange = glm::normalize(qPitch * qYaw);
	quaternion = glm::normalize(quatChange * quaternion);
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
	//quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	//std::cout << glm::to_string(front) << std::endl;
	front = quaternion * glm::vec3(0.0f, 0.0f, -1.0f) * glm::conjugate(quaternion);
	front = glm::normalize(front);
	//std::cout << glm::to_string(front) << std::endl;
	//std::cout << "***********" << std::endl;
	std::cout << glm::to_string(front)<<std::endl;
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	
}
