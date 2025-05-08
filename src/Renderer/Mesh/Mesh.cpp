#include "Mesh.h"
#include <GL/glew.h>  // or glew.h
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    : m_VBO(vertices), m_EBO(indices), m_IndexCount(static_cast<GLsizei>(indices.size()))
{
    m_VAO.Bind();
    m_VBO.Bind();
    m_EBO.Bind();

    // Link the VBO attributes
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // Position
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // Normal
    m_VAO.LinkVBO(m_VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color)); // Color
    m_VAO.LinkVBO(m_VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV)); // Texture coordinates

    m_VAO.Unbind();
}

void Mesh::Draw()
{
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
    m_VAO.Unbind();
}

void Mesh::SetTransform(const glm::mat4& transform)
{
    m_Transform = transform;
}