#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "../Graphics/Camera.h"

struct CameraComponent
{
	CameraComponent() {}

	CameraComponent(glm::vec3 position, float speed, float sensitivity) :
		Camera(position, speed, sensitivity)
	{
	
	}

	Camera Camera;

	float FOV;
	float AspectRatio;
	float NearClip;
	float FarClip;

	float Speed;
	float Sensitivity;
	float Zoom;
};

#endif // CAMERACOMPONENT_H