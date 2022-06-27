#version 330 core

uniform mat4 V;
uniform mat4 P;

in vec3 vertex;

out vec3 iTexCoords;

void main()
{
    iTexCoords = vertex;
    gl_Position = (P * V * vec4(vertex, 1.0)).xyww;
}