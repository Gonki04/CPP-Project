#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../VBO.h"  // This includes Vertex struct
#include "../EBO.h"
#include "../VAO.h"

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
    void Draw();
    void SetTransform(const glm::mat4& transform);

private:
    VAO m_VAO;
    VBO m_VBO;
    EBO m_EBO;
    GLsizei m_IndexCount;
    glm::mat4 m_Transform;
};