#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& vbo, GLuint layout, GLuint size, GLenum type, GLsizeiptr stride, void* offset) {
	vbo.Bind();


	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);


	glEnableVertexAttribArray(layout);


	vbo.Unbind();
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

