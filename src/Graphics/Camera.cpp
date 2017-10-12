#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Camera::Camera() :
	mYaw(-90.0f),
	mPitch(0.0f),
	mMovementSpeed(10.0f),
	mMouseSensitivity(0.1f),
	mWorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

Camera::Camera(glm::vec3 position, float speed, float sensitivity) : 
	mPosition(position),
	mYaw(-90.0f),
	mPitch(0.0f),
	mMovementSpeed(speed),
	mMouseSensitivity(sensitivity),
	mWorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

void Camera::SetCameraSpeed(float speed)
{
	mMovementSpeed = speed;
}

void Camera::SetMouseSensitivity(float sensitivity)
{
	mMouseSensitivity = sensitivity;
}

void Camera::SetYaw(float yaw)
{
	mYaw = yaw;
	updateCameraVectors();
}

void Camera::SetPitch(float pitch)
{
	mPitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

glm::vec3 Camera::GetPosition()
{
	return mPosition;
}

void Camera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
{
	mProjection = glm::perspective(fov, aspectRatio, nearClip, farClip);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return mProjection;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mMouseSensitivity;
	yoffset *= mMouseSensitivity;

	mYaw += xoffset;
	mPitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	if (direction == FORWARD)
		mPosition += mFront * velocity;
	if (direction == BACKWARD)
		mPosition -= mFront * velocity;
	if (direction == LEFT_CAM)
		mPosition -= mRight * velocity;
	if (direction == RIGHT_CAM)
		mPosition += mRight * velocity;
}

void Camera::updateCameraVectors() 
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	mUp = glm::normalize(glm::cross(mRight, mFront));
}
