#version 330 core

uniform sampler2D sampler;
uniform sampler2D sampler2;

in vec3 color;
in vec2 textureCoordinates;

out vec4 outColor;

void main() {
    outColor = vec4(1.0f) - (mix(texture(sampler, textureCoordinates), texture(sampler2, textureCoordinates), 0.4) * vec4(color, 1.0f));
}
