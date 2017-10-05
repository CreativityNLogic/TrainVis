#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragPosition;
out vec3 Normal;
out vec2 UV;

uniform mat4 Model;
uniform mat4 MVP;

void main()
{
	FragPosition = vec3(Model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(Model))) * normal;
	UV = uv;
	gl_Position = MVP * vec4(position, 1.0);
}