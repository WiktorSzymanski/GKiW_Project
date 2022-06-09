#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct MovingLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	bool isOn;
};

//-------------

float attenConst = 0.01f;
float attenLinear = 0.01f;
float attenExp = 0.01f;

//-------------


in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

uniform MovingLight lightTab[1];
uniform vec3 viewPos;

uniform vec4 lightColor;

vec3 normal = normalize(Normal);

float shininess = 100.0f;
vec3 viewDir = normalize(viewPos - FragPos);


vec4 calcLight(int index) {

	if (!lightTab[index].isOn)
		return vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// Attenuation
	vec3 lightDirection = FragPos - lightTab[index].position;
	float distance = length(lightDirection);

	float attenuation = attenConst +
						attenLinear * distance +
						attenExp * distance * distance;


	// Ambient
	vec3 ambient2 = lightTab[index].ambient;

	// Diffuse
	vec3 lightDir2 = normalize(lightTab[index].position - FragPos);
	float NDotL2 = max(dot(normal, lightDir2), 0.0f);
	vec3 diffuse2 = lightTab[index].diffuse * vec3(lightColor.r, lightColor.g, lightColor.b) * NDotL2;

	// Specular
	vec3 specular2 = lightTab[index].specular;

	return vec4(ambient2 + diffuse2 + specular2 , lightColor.a) / attenuation;
}

void main() {
   fragColor = calcLight(0);
};