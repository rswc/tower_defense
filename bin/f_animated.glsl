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
uniform sampler2D texSpecular;
uniform PointLight lights[NUM_POINT_LIGHTS];

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 texCoords;
in vec3 iNormal;
in vec3 iPosition;
in vec3 iViewDir;

vec3 PointLightComponent(PointLight light, vec3 normal, vec3 viewDir, vec3 specularColor, vec3 lightDir) {
	vec3 reflectDir = reflect(-lightDir, normal);

    float diffStrength = max(dot(-normal, lightDir), 0.0);
	float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), 32);


    float r = distance(light.position, iPosition);
    float att = 1 / (r * r * light.A + r * light.B + light.C);

    vec3 total = light.ambient + light.diffuse * diffStrength + light.specular * specStrength;

    // pixelColor = vec4(vec3(diffStrength), 1.0);

    return total * att;
}

void main(void) {
	vec3 normal = normalize(iNormal);
	vec3 viewDir = normalize(iViewDir);

	vec3 dirDiffuse = dirDiffuseColor * max(dot(vec4(normal, 0.0), lightDir), 0.0);

	vec3 specColor = texture(texSpecular, texCoords).rgb;

	vec3 lightColor = dirDiffuse + dirAmbientColor;
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        lightColor += PointLightComponent(lights[i], normal, viewDir, specColor, normalize(iPosition - lights[i].position));
    }

	pixelColor = texture(tex, texCoords) * vec4(lightColor, 1.0);
    // pixelColor = vec4(normal, 1.0);
}
