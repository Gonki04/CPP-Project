#pragma once

#include "VBO.h"

namespace Render
{
	class VAO
	{
	public:
		GLuint ID; // ID do Vertex Array Object
		VAO(); // Construtor que cria o VAO e gera um ID

		void LinkVBO(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset); // Liga um VBO ao VAO com os parâmetros especificados
		void Bind(); // Liga o VAO para uso
		void Unbind(); // Desliga o VAO
		void Delete(); // Apaga o VAO
	};

}