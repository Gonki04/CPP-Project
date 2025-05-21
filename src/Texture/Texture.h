#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <map>

struct Material {
    glm::vec3 ambient = glm::vec3(0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f);
    glm::vec3 specular = glm::vec3(0.0f);
    float shininess = 0.0f;
};

class Texture {
public:
    Texture();
    ~Texture();

    static unsigned int LoadTexture(const std::string& path);
    static void LoadMTL(const std::string& mtlFilePath, std::map<std::string, Material>& materials);
    static std::string GetTexturePath(const std::string& basePath, const std::string& textureName);

private:
    static void SetupTextureParameters(unsigned int textureID);
};
#pragma once
