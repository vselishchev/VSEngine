#version 430 core

layout (location = 0) in vec4 position;

out VS_OUT
{
	vec3 color;
	//vec3 normal;
} vsOut;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

void main()
{
	gl_Position = projMatrix * mvMatrix * position;
	//vsOut.normal = gl_Normal;
	vsOut.color = vec3(0.0, 1.0, 0.0);
}