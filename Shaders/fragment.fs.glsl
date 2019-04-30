#version 430 core

out vec4 color;

in VS_OUT 
{
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} fsIn;

uniform vec3 meshColor;

struct Light
{
	vec3 position;
	vec3 direction; 

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation parameters
	float constant;
	float linear;
	float quadratic;

	// Spotlight parameter
	float cutOff;
	float outerCutOff;

	int type; // 1 - directional, 2 - point, 3 - spotlight
};

uniform Light light;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform Material material;

void main()
{
	vec3 diffuseTex = texture(material.diffuseMap, fsIn.textureCoord).rgb;

	vec3 normal = normalize(fsIn.normal);
	float attenuation = 1.0;
	
	vec3 lightDir = vec3(1.0);
	if (light.type == 1)
	{
		lightDir = -normalize(light.direction);
	}
	else
	{
		float distance = length(light.position - fsIn.fragmentPosition);
		attenuation = 1.0 / (light.constant + 
							 distance * light.linear + 
							 distance * distance * light.quadratic);

		lightDir = normalize(light.position - fsIn.fragmentPosition);
	}

	// Ambient part
	vec3 ambient = light.ambient * diffuseTex; //material.ambient;

	// Diffuse part
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseTex; //material.diffuse;

	// Specular part
	vec3 viewDir = -normalize(fsIn.fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specularMap, fsIn.textureCoord).rgb; //material.specular;

	vec4 result = vec4((diffuse + ambient + specular) * attenuation /** meshColor*/, 1.0);

	if (light.type == 3)
	{
		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		result = vec4((diffuse * intensity + ambient + specular * intensity) * attenuation, 1.0);
		
	}

	color = result;
}