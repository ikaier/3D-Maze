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
	CameraZ = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	update();

}

glm::vec3 Camera::getCameraPosition()
{
	return glm::vec3();
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::vec3();
}

glm::vec4 Camera::getViewMatrix()
{
	return glm::vec4();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W]) {
		position += CameraZ * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += CameraX * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= CameraX * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= CameraZ * velocity;
	}
	if (keys[GLFW_KEY_E]) {
		flip(deltaTime);
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
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
	glm::vec3 EulerAngles(pitch, yaw, roll);
	glm::quat quaternion= glm::quat(EulerAngles);
	
}
