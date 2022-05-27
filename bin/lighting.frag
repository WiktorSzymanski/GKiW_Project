#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main() {
	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float NDotL = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
	//vec3 diffuse = light.diffuse * vec3(1.0f, 1.0f, 1.0f) * NDotL;

	// Specular (Blinn-Phong)
	float shininess = 100.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	fragColor = vec4(ambient + diffuse + specular, 1.0f);
};