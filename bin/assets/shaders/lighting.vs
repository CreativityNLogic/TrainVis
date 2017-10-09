#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;
layout (location = 5) in vec3 colour;

out vec3 FragPosition;
out mat3 TBN;
out vec2 UV;
out vec3 DistanceVector;

uniform mat4 Model;
uniform vec3 ViewPosition;
uniform mat4 MVP;

void main()
{
	FragPosition = vec3(Model * vec4(position, 1.0));
	
	mat3 normalMatrix = transpose(inverse(mat3(Model)));
	
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	
	TBN = transpose(mat3(T, B, N));
	UV = uv;
	
	DistanceVector = ViewPosition - position;
	
	gl_Position = MVP * vec4(position, 1.0);
}