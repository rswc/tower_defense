#version 330 core
in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 P;

void main()
{
    gl_Position = P * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}