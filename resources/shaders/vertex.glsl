#version 330 core

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTextureCoordinates;

out vec2 textureCoordinates;

void main() {
    textureCoordinates = inTextureCoordinates;
    gl_Position = projectionMatrix * modelMatrix * vec4(position, 1.0);
}
