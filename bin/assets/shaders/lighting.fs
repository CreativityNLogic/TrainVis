#version 330 core

in vec3 FragPosition;
in vec4 EyePosition;
in vec2 UV;
in mat3 TBN;

out vec4 FragColor;

uniform vec4 DiffuseColour;
uniform vec4 SpecularColour;
uniform vec4 AmbientColour;

uniform vec3 ViewPosition;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;
uniform sampler2D EmissiveTexture;

uniform int Shininess;

uniform float Opacity;

uniform bool IsTransparent;
uniform bool UseFog;

struct Lights 
{
	int type;
	
	vec3 position;
    vec3 direction;
	
    vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	
	float ambientStrength;
	float specularStrength;
	
	float constant;
	float linear;
	float quadratic;
	
	float innerCutoff;
	float outerCutoff;
};  

#define MAXLIGHTS 1  
uniform Lights lights[MAXLIGHTS];

vec4 LightingCalculation(int i)
{
	vec3 normal = texture(NormalTexture, UV).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 lightPosition = TBN * lights[i].position;
	vec3 fragPos = TBN * FragPosition;
	vec3 viewPos = TBN * ViewPosition;
	
	vec3 lightDirection = normalize(-lights[i].direction);
	
	if(lights[i].type != 0)
		lightDirection = normalize(lightPosition - fragPos);
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfDir = normalize(lightDirection + viewDir);
	
	vec3 ambient = texture(DiffuseTexture, UV).rgb * lights[i].diffuse * lights[i].ambientStrength;
	vec3 diffuse = texture(DiffuseTexture, UV).rgb * lights[i].specular * max(dot(normal, lightDirection), 0.0);
	vec3 specular = texture(SpecularTexture, UV).rgb * lights[i].ambient * pow(max(dot(normal, halfDir), 0.0), 256);
	vec3 emissive = texture(EmissiveTexture, UV).rgb;
	
	float theta = dot(lightDirection, normalize(lights[i].direction));
	float epsilon = lights[i].innerCutoff - lights[i].outerCutoff;
	float intensity = clamp((theta - lights[i].outerCutoff) / epsilon, 0.0, 1.0);
	
	if(lights[i].type == 2)
	{
		diffuse *= intensity;
		specular *= intensity;	
	}
	
	float distance = length(lightPosition - fragPos);
	float attenuation = 1.0 / (lights[i].constant + (lights[i].linear * distance) + (lights[i].quadratic * (distance * distance)));
	
	if(lights[i].type != 0)
	{
		diffuse *= attenuation;
		specular *= attenuation;
	}	
	
	return vec4(emissive + ambient + diffuse + specular, 1.0);
}

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

float calculateFog(FogParameters params, float fogCoord)
{
	float result = 0.0;
	if(params.type == 0)
		result = (params.linearEnd - fogCoord) / (params.linearEnd - params.linearStart);
	else if(params.type == 1)
		result = exp(-params.expDensity * fogCoord);
	else if(params.type == 2)
		result = exp(-pow(-params.expDensity * fogCoord, 2.0));
		
	return (1.0 - clamp(result, 0.0, 1.0));
}

void main()
{
	vec4 result = vec4(0.0);
	
	if(!IsTransparent)
	{
		for(int i = 0; i < MAXLIGHTS; i++)
			result = result + LightingCalculation(i);
	}
	else
	{
		vec4 diffuse = texture(DiffuseTexture, UV);
		result = vec4(diffuse.rgb, diffuse.a * Opacity);
	}
	
	if(UseFog)
	{	
		float fogCoord = abs(EyePosition.z / EyePosition.w); 
		result = mix(result, fogParameters.colour, calculateFog(fogParameters, fogCoord));	
	}
	
	FragColor = vec4(result.rgb, 1.0);
}