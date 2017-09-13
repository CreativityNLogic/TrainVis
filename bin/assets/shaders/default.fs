#version 330 core

out vec4 FragColor;

uniform vec4 DiffuseColour;

void main()
{
   FragColor = DiffuseColour;
}