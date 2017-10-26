#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

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

struct BoneInfo
{
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransformation;
};

#endif // VERTEXDATA_H