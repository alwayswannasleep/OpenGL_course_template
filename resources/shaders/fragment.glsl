#version 330 core

uniform sampler2D sampler;
uniform sampler2D sampler2;
uniform vec3 cameraPosition;
uniform vec3 cameraDirection;

in vec2 textureCoordinates;
in vec3 normal;
in vec3 objectPosition;

out vec4 outColor;

void main() {
    vec3 diffuseColor = mix(texture(sampler, textureCoordinates), texture(sampler2, textureCoordinates), 0.4).rgb;
    vec3 ambientColor = diffuseColor * 0.1;
    vec3 specularColor = vec3(1.0, 1.0, 1.0);

    vec3 lightPosition = cameraPosition;
    vec3 lightDirection = normalize(objectPosition - lightPosition);

    float diffuseFactor = max(dot(normal, -lightDirection), 0.0);
    diffuseColor = diffuseFactor * diffuseColor;

    vec3 viewDirection = normalize(objectPosition - cameraPosition);
    vec3 reflectDirection = reflect(lightDirection, normal);

    float shininess = 32;
    float specularFactor = pow(max(dot(-viewDirection, reflectDirection), 0.0), shininess);
    specularColor = 0.5 * specularColor * specularFactor;

    float phi = cos(radians(12.0));
    float outerPhi = cos(radians(17.0));

    float theta = dot(normalize(cameraDirection), lightDirection);
    float epsilon = phi - outerPhi;
    float intensity = clamp((theta - outerPhi) / epsilon, 0.0, 1.0);

    diffuseColor *= intensity;
    specularColor *= intensity;

    float lightDistance = distance(lightPosition, objectPosition);
    float attenuation = 1 / (1 + 0.22 * lightDistance + 0.2 * lightDistance * lightDistance);

    diffuseColor *= attenuation;
    specularColor *= attenuation;
    ambientColor *= attenuation;

    vec3 result = diffuseColor + ambientColor + specularColor;
    outColor = vec4(result, 1.0);
}
