#version 330 core

uniform samplerCube skybox;

in vec3 iTexCoords;

out vec4 pixelColor;

void main()
{    
    pixelColor = texture(skybox, iTexCoords);
}