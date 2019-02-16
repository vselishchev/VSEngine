#version 430 core

out vec4 color;

uniform vec3 lightColor;
uniform vec3 lightPosition;

in VS_OUT 
{
	vec3 color;
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} fsIn;

uniform sampler2D textureSample;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = lightColor * ambientStrength;

	vec3 normal = normalize(fsIn.normal);
	vec3 lightDir = normalize(lightPosition - fsIn.fragmentPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightDir;

	vec4 textureColor = texture(textureSample, fsIn.textureCoord);
	vec4 result = mix(textureColor, vec4(ambient + diffuse, 1.0), 0.5);

	color = result;
}