#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec4 normal;

out vec4 iNormal;

void main(void) {
    iNormal = normalize(M * normal);
    gl_Position = P * V * M * vertex;
}