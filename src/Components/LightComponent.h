#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include <glm/vec3.hpp>

struct LightComponent
{
	enum Type
	{
		DIRECTIONAL = 0,
		POINT = 1,
		SPOTLIGHT = 2
	} LightType;

	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;

	float AmbientStrength;
	float SpecularStrength;

	float Constant;
	float Linear;
	float Quadratic;

	float InnerCutoff;
	float OuterCutoff;
};

#endif // LIGHTCOMPONENT_H