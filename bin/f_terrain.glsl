#version 330

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float A;
    float B;
    float C;
};

#define NUM_POINT_LIGHTS 1

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec4 lightDir;
uniform sampler2DArray texMain;
uniform PointLight lights[NUM_POINT_LIGHTS];

in vec4 iNormal;
in vec3 iTexture;
in vec3 iPosition;

out vec4 pixelColor;

vec3 PointLightComponent(PointLight light, vec3 lightDir) {
    float diffStrength = max(dot(iNormal, vec4(normalize(lightDir), 0.0)), 0.0);

    float r = distance(light.position, iPosition);
    float att = 1 / (r * r * light.A + r * light.B + light.C);

    vec3 total = light.ambient + light.diffuse * diffStrength;

    return total * att;
}

void main(void) {
    // we can assume the normals are constant within a face
    vec3 diffuse = diffuseColor * max(dot(iNormal, lightDir), 0.0);

    vec4 terrainColorA = texture(texMain, vec3(iPosition.xz, iTexture[0]));
    vec4 terrainColorB = texture(texMain, vec3(iPosition.xz, iTexture[1]));
    vec4 terrainColor = iTexture[2] * terrainColorA + (1 - iTexture[2]) * terrainColorB;

    vec3 lightColor = diffuse + ambientColor;
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        lightColor += PointLightComponent(lights[i], iPosition - lights[i].position);
    }

    pixelColor = terrainColor * vec4(lightColor, 1.0);
}
