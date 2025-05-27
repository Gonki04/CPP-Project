#pragma once

#include <GL/glew.h>
#include <vector>

namespace Render
{
	class EBO
	{
	public:
		GLuint ID; // ID do Element Buffer Object
		EBO(); // Construtor que cria o EBO e gera um ID
		~EBO(); // Destrutor que apaga o EBO

		void BufferData(const std::vector<GLuint> &indices); // Envia os dados dos índices para o EBO
		void Bind(); // Liga o EBO para uso
		void Unbind(); // Desliga o EBO
	};

}