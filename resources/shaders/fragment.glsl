#version 330 core

uniform sampler2D sampler;
uniform sampler2D sampler2;
uniform vec3 cameraPosition;

in vec2 textureCoordinates;
in vec3 normal;
in vec3 objectPosition;

out vec4 outColor;

void main() {
    vec3 diffuseColor = mix(texture(sampler, textureCoordinates), texture(sampler2, textureCoordinates), 0.4).rgb;
    vec3 ambientColor = diffuseColor * 0.05;
    vec3 specularColor = vec3(1.0, 1.0, 1.0);

    vec3 lightPosition = vec3(90.0, 80.0, 50.0);
    vec3 lightDirection = normalize(objectPosition - lightPosition);

    float diffuseFactor = max(dot(normal, -lightDirection), 0.0);
    diffuseColor = diffuseFactor * diffuseColor;

    vec3 viewDirection = normalize(objectPosition - cameraPosition);
    vec3 reflectDirection = reflect(lightDirection, normal);

    float specularFactor = pow(max(dot(-viewDirection, reflectDirection), 0.0), 32);
    specularColor = 0.5 * specularColor * specularFactor;

    outColor = vec4(diffuseColor + ambientColor + specularColor, 1.0);
}
