#version 330 core

in vec3 FragPosition;
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

uniform bool UseLight;

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

void main()
{
	vec4 result;
	
	if(UseLight)
	{
		vec3 normal = texture(NormalTexture, UV).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		
		for(int i = 0; i < MAXLIGHTS; i++)
		{
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
			
			result = result + vec4(emissive + ambient + diffuse + specular, 1.0);
		}
	}
	else
	{
		result = texture(DiffuseTexture, UV);
	}
	
	FragColor = result;
}