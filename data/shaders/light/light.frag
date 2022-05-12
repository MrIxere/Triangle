#version 300 es
precision highp float;

layout (location = 0) out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 texCoords;

struct Light {
    vec3 position;
  
    vec3 ambientL;
    vec3 diffuseL;
    vec3 specularL;
};

struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
};  
  
uniform Light light; 
uniform Material material;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {


    //ambient
   
    vec3 ambient = light.ambientL  * vec3(texture(material.diffuse, texCoords));

    //Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuseL * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
    vec3 specular = light.specularL * (spec * material.specular);   
  

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}