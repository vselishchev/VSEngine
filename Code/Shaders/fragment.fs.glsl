#version 430 core

out vec4 color;

in VS_OUT 
{
	vec3 normal;
	vec3 fragmentPosition;
	vec2 textureCoord;
} fsIn;

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation parameters
	float constant;
	float linear;
	float quadratic;	
};
#define POINT_LIGHTS_COUNT 1

struct Spotlight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation parameters
	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

struct Material
{
	sampler2D diffuseMap1;
	sampler2D diffuseMap2;
	sampler2D diffuseMap3;
	sampler2D specularMap1;
	sampler2D specularMap2;
	sampler2D specularMap3;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform vec3 meshColor;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
//uniform Spotlight flashlight;

vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 viewDir, 
							   vec4 diffuseTex, vec4 specularTex);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragmentPosition, 
						 vec3 viewDir, vec4 diffuseTex, vec4 specularTex);
vec3 CalculateSpotLight(Spotlight spotlight, vec3 normal, vec3 fragmentPosition, 
						vec3 viewDir, vec4 diffuseTex, vec4 specularTex);

void main()
{
	vec3 viewDir = -normalize(fsIn.fragmentPosition);
	vec3 normal = normalize(fsIn.normal);
	vec4 diffuseTex = texture(material.diffuseMap1, fsIn.textureCoord);
	//if (diffuseTex.a < 0.01)
	//{
	//	discard;
	//}

	vec4 specularTex = texture(material.specularMap1, fsIn.textureCoord);

	vec3 outputColor = CalculateDirectionalLight(directionalLight, normal, viewDir, 
											 	 diffuseTex, specularTex);

	for (int i = 0; i < POINT_LIGHTS_COUNT; ++i)
	{
		outputColor += CalculatePointLight(pointLights[i], normal, fsIn.fragmentPosition, 
										   viewDir, diffuseTex, specularTex);
	}


	// Possible to define also spotlight
	/*outputColor += CalculateSpotLight(flashlight, normal, fsIn.fragmentPosition, 
									  viewDir, diffuseTex, specularTex);*/


	color = vec4(outputColor, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 viewDir, 
							   vec4 diffuseTex, vec4 specularTex)
{
	vec3 lightDir = normalize(-dirLight.direction);

	// Diffuse component
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	// Total
	vec3 ambient = dirLight.ambient * diffuseTex.rgb;
	vec3 diffuse = dirLight.diffuse * diff * diffuseTex.rgb;
	vec3 specular = dirLight.specular * spec * specularTex.rgb;

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragmentPosition, 
						 vec3 viewDir, vec4 diffuseTex, vec4 specularTex)
{
	float distance = length(pointLight.position - fragmentPosition);
	float attenuation = 1.0 / (pointLight.constant + 
							   distance * pointLight.linear + 
							   distance * distance * pointLight.quadratic);

	vec3 lightDir = normalize(pointLight.position - fragmentPosition);

	// Diffuse component
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	// Total
	vec3 ambient = pointLight.ambient * diffuseTex.rgb;
	vec3 diffuse = pointLight.diffuse * diff * diffuseTex.rgb;
	vec3 specular = pointLight.specular * spec * specularTex.rgb;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(Spotlight spotlight, vec3 normal, vec3 fragmentPosition, 
						vec3 viewDir, vec4 diffuseTex, vec4 specularTex)
{
	float distance = length(spotlight.position - fragmentPosition);
	float attenuation = 1.0 / (spotlight.constant + 
							   distance * spotlight.linear + 
							   distance * distance * spotlight.quadratic);

	vec3 lightDir = normalize(spotlight.position - fragmentPosition);

	// Diffuse component
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular component
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	// spotlight attenuation
	float theta = dot(lightDir, normalize(-spotlight.direction));
	float epsilon = spotlight.cutOff - spotlight.outerCutOff;
	float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

	// Total
	vec3 ambient = spotlight.ambient * diffuseTex.rgb;
	vec3 diffuse = spotlight.diffuse * diff * diffuseTex.rgb * intensity;
	vec3 specular = spotlight.specular * spec * specularTex.rgb * intensity;

	return (ambient + diffuse + specular) * attenuation;
}