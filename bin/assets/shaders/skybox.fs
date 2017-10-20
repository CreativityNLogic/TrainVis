#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube CubeMap;
uniform bool UseFog;

struct FogParameters 
{ 
	int type;
	vec4 colour; 
	float linearStart;
	float linearEnd;
	float expDensity;
	float skyMix;
}; 

uniform FogParameters fogParameters;

void main()
{   
	vec4 result;
		
	result = texture(CubeMap, TexCoords);
	
	if(UseFog)
	{
		result = mix(result, fogParameters.colour, fogParameters.skyMix);	
	}
		
    FragColor = result;
}