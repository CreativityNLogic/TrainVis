#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
///layout (location = 4) in vec2 aUV;

out vec3 Normal;
out vec3 Position;
out vec2 UV;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(Model * vec4(aPos, 1.0));
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);

//	UV = aUV;
}