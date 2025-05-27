#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include "stb_image/stb_image.h"

struct Material {
    glm::vec3 ambient = glm::vec3(0.0f); // Cor ambiente
    glm::vec3 diffuse = glm::vec3(0.0f); // Cor difusa
    glm::vec3 specular = glm::vec3(0.0f); // Cor especular
    float shininess = 0.0f; // Brilho especular
    unsigned int diffuseMap = 0; // ID da textura difusa
};

class Texture {
public:
    unsigned int id; // ID da textura OpenGL
    std::string type; // Tipo de textura 

    Texture(); 
    ~Texture(); 

    static unsigned int LoadTexture(const std::string& path); // Carrega uma textura a partir de um caminho
    static void LoadMTL(const std::string& mtlFilePath, std::map<std::string, Material>& materials); // Carrega um ficheiro MTL e armazena os materiais num mapa
    static std::string GetTexturePath(const std::string& basePath, const std::string& textureName); // Obtém o caminho completo da textura a partir do caminho base e do nome da textura

private:
    static void SetupTextureParameters(unsigned int textureID); // Configura os parâmetros da textura OpenGL
};
