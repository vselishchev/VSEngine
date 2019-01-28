#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT
{
	vec3 color;
	vec3 normal;
	vec3 fragmentPosition;
} vsOut;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

void main()
{
	gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
	vsOut.normal = mat3(mvMatrix) * normal;
	vsOut.fragmentPosition = vec3(mvMatrix * vec4(position, 1.0));
	vsOut.color = vec3(0.0, 1.0, 0.0);
}