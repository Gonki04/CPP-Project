#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

struct Material {
    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
    float shininess = 0.0f;
    unsigned int diffuseMap = 0;
};

class Texture {
public:
    unsigned int id;
    std::string type;

    Texture();
    ~Texture();

    static unsigned int LoadTexture(const std::string& path);
    static void LoadMTL(const std::string& mtlFilePath, std::map<std::string, Material>& materials);
    static std::string GetTexturePath(const std::string& basePath, const std::string& textureName);

private:
    static void SetupTextureParameters(unsigned int textureID);
};
