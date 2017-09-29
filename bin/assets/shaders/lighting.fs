#version 330 core

in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform vec4 DiffuseColour;
uniform sampler2D DiffuseTexture;

void main()
{
	FragColor = texture(DiffuseTexture, UV);
}