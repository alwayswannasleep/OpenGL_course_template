#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTextureCoordinates;

out vec3 color;
out vec2 textureCoordinates;

void main() {
    color = inColor;
    textureCoordinates = inTextureCoordinates;
    gl_Position = vec4(position, 1.0);
}
