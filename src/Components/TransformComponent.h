#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/vec3.hpp>

struct TransformComponent
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

#endif // TRANSFORMCOMPONENT_H