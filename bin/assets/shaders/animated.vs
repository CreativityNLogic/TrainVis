#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;
layout (location = 5) in vec3 colour;
layout (location = 6) in ivec4 boneIDs;
layout (location = 7) in vec4 weights;

out vec3 FragPosition;
out vec4 EyePosition;
out mat3 TBN;
out vec2 UV;

uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 MVP;

const int MAX_JOINTS = 100;
const int MAX_WEIGHTS = 4;

uniform mat4 bones[MAX_JOINTS];

void main()
{
	// ANIMATIONS - START

	// ANIMATIONS - END
	
	EyePosition = View * Model * vec4(position, 1.0);
	FragPosition = vec3(Model * vec4(position, 1.0));
	
	mat3 normalMatrix = transpose(inverse(mat3(Model)));
	
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	
	TBN = transpose(mat3(T, B, N));
	UV = uv;
	
	gl_Position = MVP * vec4(position, 1.0);
}