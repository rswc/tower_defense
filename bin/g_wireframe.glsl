#version 330

layout (triangles) in;
layout (line_strip) out;
layout (max_vertices=4) out;

in vec4 gColor[];

out vec4 iColor;

void main(void) {
    int i;
    for (i=0; i<gl_in.length(); i++) {
        gl_Position=gl_in[i].gl_Position;
        iColor=gColor[i];
        EmitVertex();
    }

    gl_Position=gl_in[0].gl_Position;
    iColor=gColor[0];
    EmitVertex();
    
    EndPrimitive();
}