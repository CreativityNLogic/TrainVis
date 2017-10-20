#ifndef FOGCOMPONENT_H
#define FOGCOMPONENT_H

#include <glm/glm.hpp>

struct FogComponent
{
	FogComponent() : Enabled(false) {}

	FogComponent(const FogComponent& fog)
	{
		Type = fog.Type;
		Colour = fog.Colour;
		LinearStart = fog.LinearStart;
		LinearEnd = fog.LinearEnd;
		ExpDensity = fog.ExpDensity;
		SkyMix = fog.SkyMix;
		Enabled = fog.Enabled;
	}

	enum Type
	{
		LINEAR = 0,
		EXP,
		EXP2
	} Type;

	glm::vec4 Colour;

	float LinearStart;
	float LinearEnd;
	float ExpDensity;
	float SkyMix;

	bool Enabled;
};

#endif // FOGCOMPONENT_H