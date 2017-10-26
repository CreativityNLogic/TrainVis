#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <glm/glm.hpp>

#define NUM_BONES_PER_VEREX 4

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
	glm::vec2 TexCoords;
	glm::vec3 Colour;
	glm::ivec4 BoneIDs;
	glm::vec4 VertexWeights;
};

#endif // VERTEXDATA_H