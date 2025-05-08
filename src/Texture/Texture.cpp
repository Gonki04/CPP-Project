//#include "Texture.h"
//#include <stb_image.h>
//#include <iostream>
//
//Texture::Texture(const std::string& filePath, GLenum slot)
//    : m_Slot(slot) {
//    // Generate and bind texture
//    glGenTextures(1, &m_ID);
//    LoadFromFile(filePath);
//}
//
//void Texture::LoadFromFile(const std::string& filePath) {
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
//
//    if (data) {
//        GLenum format;
//        switch (m_NrChannels) {
//        case 1: format = GL_RED; break;
//        case 3: format = GL_RGB; break;
//        case 4: format = GL_RGBA; break;
//        default: format = GL_RGB; break;
//        }
//
//        Bind();
//        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        // Set texture parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else {
//        std::cerr << "Failed to load texture: " << filePath << std::endl;
//        stbi_image_free(data);
//    }
//}
//
//void Texture::Bind() const {
//    glActiveTexture(m_Slot);
//    glBindTexture(GL_TEXTURE_2D, m_ID);
//}
//
//void Texture::Unbind() const {
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Texture::SetSlot(GLenum newSlot) {
//    m_Slot = newSlot;
//}
//
//Texture::~Texture() {
//    glDeleteTextures(1, &m_ID);
//}