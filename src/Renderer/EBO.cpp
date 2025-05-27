#include "EBO.h"

namespace Render
{
    // cria o EBO
    EBO::EBO()
    {
        glGenBuffers(1, &ID);
    }

    // destroi o EBO assim que a classe é destruida
    EBO::~EBO()
    {
        glDeleteBuffers(1, &ID);
    }

    // alloca memoria para o os indices
    void EBO::BufferData(const std::vector<GLuint> &indices)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    // dá bind ao EBO
    void EBO::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    // dá unbind ao EBO
    void EBO::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}