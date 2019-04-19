#version 430 core

out vec4 color;

in VS_OUT 
{
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} fsIn;

uniform vec3 meshColor;

uniform sampler2D textureSample;

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
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{
	// ambient part
	vec3 ambient = light.ambient * material.ambient;

	// diffuse part
	vec3 normal = normalize(fsIn.normal);
	vec3 lightDir = normalize(light.position - fsIn.fragmentPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular part
	vec3 viewDir = -normalize(fsIn.fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	//vec4 textureColor = texture(textureSample, fsIn.textureCoord);
	vec4 result = vec4((diffuse + ambient + specular) * meshColor, 1.0);

	color = result;
}