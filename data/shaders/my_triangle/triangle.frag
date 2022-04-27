/*#version 310 es
precision highp float;

in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main(){
	outColor = vec4(fragColor, 1.0f);
}
*/

#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}