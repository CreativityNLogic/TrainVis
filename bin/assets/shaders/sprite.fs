#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D Sprite;
uniform float Opacity;

void main()
{
	vec4 diffuse = texture(Sprite, UV);
	FragColor = vec4(diffuse.rgb * diffuse.a, diffuse.a);
}