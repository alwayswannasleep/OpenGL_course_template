#version 330 core

uniform sampler2D sampler;
uniform sampler2D sampler2;

in vec2 textureCoordinates;

out vec4 outColor;

void main() {
    outColor = mix(texture(sampler, textureCoordinates), texture(sampler2, textureCoordinates), 0.4);
}
