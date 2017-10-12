#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;
layout (location = 5) in vec3 colour;

out vec2 UV;

uniform mat4 MVP;
uniform mat4 VP;

uniform vec3 SpriteCenter;
uniform vec3 CameraRightWorldSpace;
uniform vec3 CameraUpWorldSpace;
uniform vec2 BillboardSize;

uniform bool IsBillboard;

void main()
{
	UV = uv;
	
	vec3 pos = SpriteCenter + (CameraRightWorldSpace * position.x * BillboardSize.x) + (CameraUpWorldSpace * position.y * BillboardSize.y);

	gl_Position = IsBillboard ? VP * vec4(pos, 1.0) : MVP * vec4(position, 1.0);
}