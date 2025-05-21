#version 450 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

uniform vec3 lightPos; 
uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;


void main() {
    
    vec3 ambient = material.ambient * Color;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // Directional light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * Color;

    vec3 viewDir = normalize(viewPos - FragPos); // Camera direction

    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = Color * (material.specular * spec);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}