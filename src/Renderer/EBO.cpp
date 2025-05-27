#include "EBO.h"

namespace Render {

EBO::EBO() {
    glGenBuffers(1, &ID);
}
EBO::~EBO() {
    // If you have a valid OpenGL buffer, delete it here
    glDeleteBuffers(1, &ID);
}

void EBO::BufferData(const std::vector<GLuint>& indices) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}