#version 330

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 lightDir;

in vec4 iNormal;
in vec3 iTexture;

out vec4 pixelColor;

void main(void) {
    // we can assume the normals are constant within a face
    vec4 diffuse = max(dot(iNormal, lightDir), 0.0) * diffuseColor;

    vec4 objectColor = vec4(iTexture, 1.0);

    pixelColor = objectColor * (diffuse + ambientColor);
}
