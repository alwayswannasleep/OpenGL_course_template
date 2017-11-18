#version 330 core

uniform vec2 uResolution;

in vec3 vertexColor;
in vec4 pos;

out vec4 color;

void main() {
     float radius = length(gl_FragCoord.xy - (vec2(0.5) + pos.xy / vec2(2)) * uResolution);

     if (radius < 75) {
        color = vec4(vertexColor, 1.0);
     } else {
        discard;
     }
}
