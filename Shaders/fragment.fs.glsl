#version 430 core

out vec4 color;

in VS_OUT 
{
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} fsIn;

uniform vec3 meshColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform sampler2D textureSample;

void main()
{
	// ambient part
	float ambientStrength = 0.1;
	vec3 ambient = lightColor * ambientStrength;

	// diffuse part
	vec3 normal = normalize(fsIn.normal);
	vec3 lightDir = normalize(lightPosition - fsIn.fragmentPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular part
	float specularStrength = 0.5;
	vec3 viewDir = -normalize(fsIn.fragmentPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	//vec4 textureColor = texture(textureSample, fsIn.textureCoord);
	vec4 result = vec4((diffuse + ambient + specular) * meshColor, 1.0);

	color = result;
}