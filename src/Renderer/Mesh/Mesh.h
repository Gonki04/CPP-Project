#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../VBO.h"  // This includes Vertex struct
#include "../EBO.h"
#include "../VAO.h"

namespace ourSpace
{
class Mesh {
public:
    bool Load(const std::string& obj_model_filepath); // Corrigido para corresponder à implementação
    void Draw();
    void SetTransform(const glm::mat4& transform);
    void Install(); // Corrigido o nome

private:
    VAO m_VAO;
    VBO m_VBO;
    EBO m_EBO;
    GLsizei m_IndexCount;
    glm::mat4 m_Transform;

    std::vector<Vertex> m_Vertices;   // Adicionado: guarda os vértices carregados
    std::vector<GLuint> m_Indices;    // Adicionado: guarda os índices carregados
};
}