#version 300 es
precision mediump float;

layout(location = 0) out vec4 fragColor;
in vec3 outColor;

uniform float colorCoeff;

void main(){
	fragColor = vec4(outColor * colorCoeff, 1.0);
}

