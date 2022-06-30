#version 330

	
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 cameraPos;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texCoord; //wsp�rz�dne teksturowania
in vec4 normal;

//Zmienne interpolowane
out vec2 iTexCoords;
out vec3 iNormal;
out vec3 iPosition;
out vec3 iViewDir;

void main(void) {
    vec4 worldPosition = M * vertex;

    iPosition = worldPosition.xyz;
    iNormal = (M * normal).xyz;
    iViewDir = cameraPos - worldPosition.xyz;
    iTexCoords = texCoord;

    gl_Position = P * V * worldPosition;
}
