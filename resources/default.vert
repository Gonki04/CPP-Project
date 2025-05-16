#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoord;

void main() {
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
    
    gl_Position = u_View * u_Projection * u_Model * vec4(FragPos, 1.0);
}