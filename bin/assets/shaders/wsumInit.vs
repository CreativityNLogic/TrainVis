#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;
layout (location = 5) in vec3 colour;

out vec2 UV;

uniform mat4 MVP;

void main()
{
	UV = uv;
	gl_Position = MVP * vec4(position, 1.0);
}