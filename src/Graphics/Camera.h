#ifndef CAMERA_H
#define CAMERA_H

#include "Glad/glad.h"
#include <glm/glm.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT_CAM,
	RIGHT_CAM
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Constructor
	Camera();

	// Constructor with vectors
	Camera(glm::vec3 position, float speed, float sensitivity);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);
	glm::mat4 GetProjectionMatrix() const;

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. 
	//Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	// Processes input received from any keyboard-like input system. 
	//Accepts input parameter in the form of camera defined ENUM 
	//(to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void SetCameraSpeed(float speed);
	void SetMouseSensitivity(float sensitivity);
	void SetZoom(float zoom);
	void SetYaw(float yaw);
	void SetPitch(float pitch);

private: 
	void updateCameraVectors();

private:
	// Camera Attributes
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	glm::mat4 mProjection;

	// Eular Angles
	float mYaw;
	float mPitch;

	// Camera options
	float mMovementSpeed;
	float mMouseSensitivity;
	float mZoom;
};

#endif // CAMERA_H