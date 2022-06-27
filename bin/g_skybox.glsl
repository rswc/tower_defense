#version 330

layout (triangles) in;
layout (line_strip) out;
layout (max_vertices=4) out;

in vec4 gTexCoords[];

out vec4 iTexgTexCoords;

void main(void) {
    int i;
    for (i=0; i<gl_in.length(); i++) {
        gl_Position=gl_in[i].gl_Position;
        iTexgTexCoords=gTexCoords[i];
        EmitVertex();
    }

    gl_Position=gl_in[0].gl_Position;
    iTexgTexCoords=gTexCoords[0];
    EmitVertex();
    
    EndPrimitive();
}