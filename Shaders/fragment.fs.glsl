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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	sampler2D specularMap;
	float shininess;
};

uniform Material material;

void main()
{
	vec3 diffuseTex = texture(material.diffuseMap, fsIn.textureCoord).rgb;
	// ambient part
	vec3 ambient = light.ambient * diffuseTex;//material.ambient;

	// diffuse part
	vec3 normal = normalize(fsIn.normal);
	vec3 lightDir = normalize(light.position - fsIn.fragmentPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * diffuseTex/*material.diffuse*/);

	// specular part
	vec3 viewDir = -normalize(fsIn.fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specularMap, fsIn.textureCoord).rgb/*material.specular*/);

	vec4 result = vec4((diffuse + ambient + specular) /**  meshColor*/, 1.0);

	color = result;
}