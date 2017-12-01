/**
 * @file	Camera.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Camera Class
 */

#include "Camera.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	:position{ position },
	front{ glm::vec3(0.f,0.f,-1.f) },
	worldUp{ up },
	yaw{ yaw },
	pitch{ pitch },
	movementSpeed{ SPEED },
	accxoffset(0.f),
	accyoffset(0.f),
	acclroffset(0.f),
	accudoffset(0.f),
	accfboffset(0.f),
	mouseSensitivity{ SENSITIVITY }
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, GLfloat timeDelta)
{
	GLfloat velocity = movementSpeed * timeDelta;
	switch (direction)
	{
	case FORWARD:
		accfboffset += velocity;
		break;
	case BACKWARD:
		accfboffset -= velocity;
		break;
	case LEFT:
		acclroffset -= velocity;
		break;
	case RIGHT:
		acclroffset += velocity;
		break;
	case ABSUP:
		accudoffset += velocity;
		break;
	case ABSDOWN:
		accudoffset -= velocity;
		break;
	}
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
	accxoffset += xOffset * mouseSensitivity;
	accyoffset += yOffset * mouseSensitivity;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(const glm::vec3& newPos)
{
	position = newPos;
}

glm::vec3 Camera::getFrontVector() const
{
	return front;
}

glm::vec3 Camera::getUpVector() const
{
	return up;
}

glm::vec3 Camera::getRightVector() const
{
	return right;
}

glm::vec3 Camera::getWorldUpVector() const
{
	return worldUp;
}

GLfloat Camera::getYaw() const
{
	return yaw;
}

GLfloat Camera::getPitch() const
{
	return pitch;
}

GLfloat Camera::getMovementSpeed() const
{
	return movementSpeed;
}

GLfloat Camera::getMouseSensitivity() const
{
	return mouseSensitivity;
}

void Camera::update()
{
	position += front * accfboffset * SMOOTH_MOVEMENT_CONSTANT;
	accfboffset *= 1 - SMOOTH_MOVEMENT_CONSTANT;

	position += right * acclroffset * SMOOTH_MOVEMENT_CONSTANT;
	acclroffset *= 1 - SMOOTH_MOVEMENT_CONSTANT;

	position += worldUp * accudoffset * SMOOTH_MOVEMENT_CONSTANT;
	accudoffset *= 1 - SMOOTH_MOVEMENT_CONSTANT;

	yaw += accxoffset * SMOOTH_MOVEMENT_CONSTANT;
	pitch += accyoffset * SMOOTH_MOVEMENT_CONSTANT;
	accxoffset *= 1 - SMOOTH_MOVEMENT_CONSTANT;
	accyoffset *= 1 - SMOOTH_MOVEMENT_CONSTANT;

	if (true)
	{
		if (pitch > 89.f)
			pitch = 89.f;
		if (pitch < -89.f)
			pitch = -89.f;
	}

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 Front;
	Front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
	Front.y = sin(glm::radians(pitch));
	Front.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
	front = glm::normalize(Front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::tempPrintCoords()
{
	std::cout << "x: " << position.x << ", y: " << position.y << ", z: " << position.z << std::endl;
}
