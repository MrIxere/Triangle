#version 330 core
precision highp float;
layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambiantStrenght = 0.1;
	vec3 ambient = ambiantStrenght *lightColor;


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir),0.0);
	vec3 diffuse = diff * lightColor;
	
	//specular
	float specularStrenght = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	vec3 specular = specularStrenght * spec * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	fragColor = vec4(result, 1.0);  
}