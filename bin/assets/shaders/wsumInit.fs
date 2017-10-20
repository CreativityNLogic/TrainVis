#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D Sprite;

void main(void)
{
	vec4 color = texture(Sprite, UV);
	FragColor = vec4(color.rgb * color.a, color.a);
}