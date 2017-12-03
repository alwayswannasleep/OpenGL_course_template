#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 inverseModelMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormal;

out vec2 textureCoordinates;
out vec3 normal;
out vec3 objectPosition;

void main() {
    textureCoordinates = inTextureCoordinates;
    normal = normalize(mat3(inverseModelMatrix) * inNormal);
    objectPosition = (modelMatrix * vec4(position, 1.0)).xyz;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
