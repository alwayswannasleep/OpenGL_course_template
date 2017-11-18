#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colorIn;

out vec3 vertexColor;

void main() {
    vertexColor = colorIn;
    gl_Position = vec4(position, 1.0);
}