#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colorIn;

out vec3 vertexColor;
out vec4 pos;

void main() {
    vertexColor = colorIn;
    pos = vec4(position, 1.0);
    gl_Position = pos;
}