#version 330

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 lightDir;

in vec4 iNormal;

out vec4 pixelColor;

void main(void) {
    // we can assume the normals are constantwithin a face
    vec4 diffuse = max(dot(iNormal, lightDir), 0.0) * diffuseColor;

    pixelColor = ambientColor * diffuse;
}
