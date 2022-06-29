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

uniform vec3 dirAmbientColor;
uniform vec3 dirDiffuseColor;
uniform vec4 lightDir;
uniform sampler2D tex;
uniform PointLight lights[NUM_POINT_LIGHTS];

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 texCoords;
in vec3 iNormal;
in vec3 iPosition;

vec3 PointLightComponent(PointLight light, vec3 normal, vec3 lightDir) {
    float diffStrength = max(dot(normal, normalize(lightDir)), 0.0);

    float r = distance(light.position, iPosition);
    float att = 1 / (r * r * light.A + r * light.B + light.C);

    vec3 total = light.ambient + light.diffuse * diffStrength;

    return total * att;
}

void main(void) {
	vec3 normal = normalize(iNormal);

	vec3 dirDiffuse = dirDiffuseColor * max(dot(vec4(normal, 0.0), lightDir), 0.0);

	vec3 lightColor = dirDiffuse + dirAmbientColor;
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        lightColor += PointLightComponent(lights[i], normal, iPosition - lights[i].position);
    }

	pixelColor = texture(tex, texCoords) * vec4(lightColor, 1.0);
}
