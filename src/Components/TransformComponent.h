#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct TransformComponent
{
	TransformComponent() :
		Position(glm::vec3(0.0f)),
		Rotation(glm::quat()),
		Scale(glm::vec3(1.0f))
	{
	
	}

	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;
};

#endif // TRANSFORMCOMPONENT_H