#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 Normal;
out vec2 UV;

uniform mat4 MVP;

void main()
{
	Normal = normal;
	UV = uv;
	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);
}