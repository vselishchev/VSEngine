#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

out VS_OUT
{
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} vsOut;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	mat4 mvMatrix = viewMatrix * modelMatrix;
	gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
	vsOut.normal = mat3(mvMatrix) * normal; //mat3(transpose(inverse(mvMatrix))) * normal; // calculate normal matrix on the CPU and send as uniform
	vsOut.fragmentPosition = vec3(mvMatrix * vec4(position, 1.0));
	vsOut.textureCoord = textureCoord;
}