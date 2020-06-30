#version 430 core
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoords;

out vec2 TexCoords;

void main()
{
	TexCoords = inTexCoords;
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
}