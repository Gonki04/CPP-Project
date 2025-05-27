#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace Render
{

	struct Vertex
	{
		glm::vec3 Position; // Posição do vértice
		glm::vec3 Normal; // Normal do vértice
		glm::vec2 TexCoords; // Coordenadas de textura do vértice
	};

	class VBO 
	{
	public:
		VBO(); // Construtor que cria o VBO e gera um ID
		GLuint ID; // ID do Vertex Buffer Object
		void BufferData(const std::vector<Vertex> &vertices); // Envia os dados dos vértices para o VBO
		void Bind(); // Liga o VBO para uso
		void Unbind(); // Desliga o VBO
		void Delete(); // Apaga o VBO
	};
}