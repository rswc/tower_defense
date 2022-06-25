#version 330

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 lightDir;
uniform sampler2DArray texMain;

in vec4 iNormal;
in vec3 iTexture;
in vec2 iUV;

out vec4 pixelColor;

void main(void) {
    // we can assume the normals are constant within a face
    vec4 diffuse = max(dot(iNormal, lightDir), 0.0) * diffuseColor;

    vec4 terrainColorA = texture(texMain, vec3(iUV, iTexture[0]));
    vec4 terrainColorB = texture(texMain, vec3(iUV, iTexture[1]));
    vec4 terrainColor = iTexture[2] * terrainColorA + (1 - iTexture[2]) * terrainColorB;

    pixelColor = terrainColor * (diffuse + ambientColor);
}
