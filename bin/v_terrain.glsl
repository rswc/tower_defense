#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec4 normal;
in vec3 texParams;

out vec4 iNormal;
out vec3 iTexture;
out vec2 iUV;

void main(void) {
    iNormal = normalize(M * normal);
    iTexture = texParams; // Used for texture layer selection and blending
    iUV = (M * vertex).xz;
    gl_Position = P * V * M * vertex;
}