#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 color;

in vec4 vertex;

out vec4 gColor;

void main(void) {
    gColor=color;
    gl_Position=P*V*M*vertex;
}