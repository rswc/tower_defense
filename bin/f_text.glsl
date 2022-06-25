#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{
    color = vec4(textColor.rgb, textColor.a * texture(text, TexCoords).r);
}