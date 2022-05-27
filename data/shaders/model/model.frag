#version 300 es
precision highp float;

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main() 
{
	fragColor = texture(texture_diffuse1, texCoords);
}