#version 430 core

out vec4 color;

uniform vec3 lightColor;
uniform vec3 lightDirection;

in VS_OUT 
{
	vec3 color;
	//vec3 normal;
} fsIn;

void main()
{
	float ambientStrength = 0.5;
	vec3 ambient = lightColor * ambientStrength;

	vec3 result = ambient * fsIn.color; 
	color = vec4(result, 1.0);
}