#version 460 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 u_ViewPos;

out vec4 FragColor;

void main() {
    // Simple textured lighting
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    
    vec3 ambient = vec3(0.1);
    vec4 texColor = texture(texture1, TexCoord);
    
    FragColor = vec4((ambient + diffuse) * texColor.rgb * Color, 1.0);
}