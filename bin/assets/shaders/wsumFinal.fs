#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform sampler2D ColorTex;
uniform vec3 BackgroundColor;

void main(void)
{
	vec4 S = texture(ColorTex, UV);
	vec4 bg = vec4(BackgroundColor * (1.0 - S.a), 1.0);
	FragColor = S;
}