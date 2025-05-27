#include "VBO.h"

namespace Render
{
	// cria o VBO
	VBO::VBO()
	{
		glGenBuffers(1, &ID);
	}

	// aloca memoria para o VBO
	void VBO::BufferData(const std::vector<Vertex> &vertices)
	{
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	// dá bind ao VBO
	void VBO::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	// dá unbind ao VBO
	void VBO::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// destroi o VBO
	void VBO::Delete()
	{
		glDeleteBuffers(1, &ID);
	}
}