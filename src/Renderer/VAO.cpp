#include "VAO.h"

namespace Render {

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& vbo, GLuint layout, GLuint size, GLenum type, GLsizeiptr stride, void* offset) {

	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);

	glEnableVertexAttribArray(layout);
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}

}