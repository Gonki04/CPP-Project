#include "Texture.h"
#include <iostream>
#include <fstream>
#include <sstream>

Texture::Texture() {}

Texture::~Texture() {}

unsigned int Texture::LoadTexture(const std::string& path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        SetupTextureParameters(textureID);

        stbi_image_free(data);
    } else {
        std::cerr << "Falha ao carregar textura no caminho: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Texture::SetupTextureParameters(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

std::string Texture::GetTexturePath(const std::string& basePath, const std::string& textureName) {
    size_t lastSlash = basePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return basePath.substr(0, lastSlash + 1) + textureName;
    }
    return textureName;
}

void Texture::LoadMTL(const std::string& mtlFilePath, std::map<std::string, Material>& materials) {
    std::fstream file(mtlFilePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo MTL: " << mtlFilePath << std::endl;
        return;
    }

    std::string line;
    Material currentMaterial;
    bool hasMaterial = false;
    std::string currentMaterialName;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "newmtl") {
            if (hasMaterial) {
                materials[currentMaterialName] = currentMaterial;
            }
            hasMaterial = true;
            iss >> currentMaterialName;
            currentMaterial = Material();
        }
        else if (prefix == "Ka") {
            iss >> currentMaterial.ambient.x >> currentMaterial.ambient.y >> currentMaterial.ambient.z;
        }
        else if (prefix == "Kd") {
            iss >> currentMaterial.diffuse.x >> currentMaterial.diffuse.y >> currentMaterial.diffuse.z;
        }
        else if (prefix == "Ks") {
            iss >> currentMaterial.specular.x >> currentMaterial.specular.y >> currentMaterial.specular.z;
        }
        else if (prefix == "Ns") {
            iss >> currentMaterial.shininess;
        }
        else if (prefix == "map_Kd") {
            std::string texturePath;
            iss >> texturePath;
            texturePath = GetTexturePath(mtlFilePath, texturePath);
            currentMaterial.diffuseMap = LoadTexture(texturePath);
        }
    }

    if (hasMaterial) {
        materials[currentMaterialName] = currentMaterial;
    }

    file.close();
}