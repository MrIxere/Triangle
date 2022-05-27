#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 atexCoords;

out vec2 texCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    texCoords = atexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}