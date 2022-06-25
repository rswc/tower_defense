#version 330 core
in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 P;
uniform vec2 offset;

void main()
{
    gl_Position = P * vec4(vertex.xy + offset, 0.0, 1.0);
    TexCoords = vertex.zw;
}