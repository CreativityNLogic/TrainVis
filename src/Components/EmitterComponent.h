#ifndef EMITTERCOMPONENT_H
#define EMITTERCOMPONENT_H

#include <glm/glm.hpp>
#include <string>

struct EmitterComponent
{
	std::string ParticleData;
	glm::vec3 Velocity;
	glm::vec3 Offset;

	float SpawnRate;
	float EmitterSpawnTime;
};

#endif // EMITTERCOMPONENT_H