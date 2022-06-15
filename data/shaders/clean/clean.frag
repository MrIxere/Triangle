#version 300 es
precision highp float;

layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 ambientL;
    vec3 diffuseL;
    vec3 specularL;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};  

uniform Light light; 
uniform Material material;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {

    vec3 ambient = light.ambientL * vec3(texture(material.diffuse, TexCoords));
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuseL * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
    vec3 specular = light.specularL * spec * vec3(texture(material.specular, TexCoords)); 
    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);

} 