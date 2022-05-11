#version 300 es
precision highp float;
layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
	float ambiantStrenght = 0.1;
	vec3 ambient = ambiantStrenght *lightColor;


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir),0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;
	

	fragColor = vec4(result, 1.0);  
}