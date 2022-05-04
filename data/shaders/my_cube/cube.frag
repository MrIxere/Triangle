#version 300 es
precision mediump float;

layout (location = 0) out vec4 fragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {

    fragColor = texture(ourTexture, TexCoord);
}