#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
	glm::vec2 TexCoords;
	glm::vec3 Colour;
	float padding[15];
};

#endif // VERTEXDATA_H