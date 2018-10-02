#version 430 core

in vec4 position;

out VS_OUT
{
	vec4 color;
} vsOut;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

void main()
{
	gl_Position = projMatrix * mvMatrix * position;
	vsOut.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}