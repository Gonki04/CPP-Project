#include "VAO.h"

namespace Render
{
	// cria o VAO
	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}

	// linka o VBO ao VAO e define os atributos do VBO
	void VAO::LinkVBO(VBO &vbo, GLuint layout, GLuint size, GLenum type, GLsizeiptr stride, void *offset)
	{
		// Define como os dados do VBO serão lidos
		glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
		// Ativa o atributo de vértice
		glEnableVertexAttribArray(layout);
	}

	// dá bind ao VAO
	void VAO::Bind()
	{
		glBindVertexArray(ID);
	}

	// dá unbind ao VAO
	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}

	// destroi o VAO
	void VAO::Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}

}