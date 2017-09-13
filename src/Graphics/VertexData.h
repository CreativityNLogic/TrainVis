#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

#endif // VERTEXDATA_H