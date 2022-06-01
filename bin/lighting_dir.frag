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
	vec3 ambient2 = lightTab[index].ambient * material.ambient;

	// Diffuse
	vec3 lightDir2 = normalize(lightTab[index].position - FragPos);
	float NDotL2 = max(dot(normal, lightDir2), 0.0f);
	vec3 diffuse2 = lightTab[index].diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL2;

	// Specular (Blinn-Phong)
	vec3 halfDir2 = normalize(lightDir2 + viewDir);
	float NDotH2 = max(dot(normal, halfDir2), 0.2f);
	vec3 specular2 = lightTab[index].specular * material.specular * pow(NDotH2, material.shininess);

	return vec4(ambient2 + diffuse2 + specular2 , 1.0f) / attenuation;
}

vec4 calcDirLight() {
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(-light.direction);
	float NDotL = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;

	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.2f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	return vec4(ambient + diffuse + specular , 1.0f);
}

void main() {
	fragColor = calcDirLight();

	for (int i = 0; i < 2; i ++) {
		fragColor += calcLight(i);
	}
};