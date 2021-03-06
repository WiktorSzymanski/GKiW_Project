#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

uniform MovingLight lightTab[2];
uniform DirectionalLight light;
uniform Material material;
uniform vec3 viewPos;

void main() {
	// Attenuation
	vec3 lightDirection = FragPos - lightTab[0].position; // ----
	float distance = length(lightDirection);

	float attenuation = attenConst +
						attenLinear * distance +
						attenExp * distance * distance;


	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	vec3 ambient2 = lightTab[0].ambient * material.ambient;

	// Diffuse
	vec3 normal = normalize(Normal);

	vec3 lightDir = normalize(-light.direction);
	vec3 lightDir2 = normalize(lightTab[0].position - FragPos); // ----

	float NDotL = max(dot(normal, lightDir), 0.0f);
	float NDotL2 = max(dot(normal, lightDir2), 0.0f);

	vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
	vec3 diffuse2 = lightTab[0].diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL2;

	// Specular (Blinn-Phong)
	float shininess = 100.0f;
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 halfDir = normalize(lightDir + viewDir);
	vec3 halfDir2 = normalize(lightDir2 + viewDir);

	float NDotH = max(dot(normal, halfDir), 0.2f);
	float NDotH2 = max(dot(normal, halfDir2), 0.2f);

	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);
	vec3 specular2 = lightTab[0].specular * material.specular * pow(NDotH2, material.shininess);

	vec4 lightOne = vec4(ambient + diffuse + specular , 1.0f);


	
	vec4 lightTwo;
	if (lightTab[0].isOn) {
		lightTwo = vec4(ambient2 + diffuse2 + specular2 , 1.0f) / attenuation;
	} else {
		lightTwo = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	fragColor = lightOne + lightTwo;
};