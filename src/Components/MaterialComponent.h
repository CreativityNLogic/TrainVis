#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H

#include "../Graphics/Material.h"

struct MaterialComponent
{
	std::vector<Material> Materials;
	bool Transparent;
};

#endif // MATERIALCOMPONENT_H