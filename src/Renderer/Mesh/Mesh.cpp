#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;


	setupMesh();
}



void Mesh::setupMesh() {
   
	if (vertices.empty() || indices.empty()) {
		std::cerr << "Mesh::setupMesh: vertices or indices are empty!" << std::endl;
		return;
	}

	m_VAO.Bind();
	m_VBO.Bind();
	m_EBO.Bind();

	m_VBO.BufferData(vertices);
	m_EBO.BufferData(indices);

	m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	m_VAO.LinkVBO(m_VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	m_VAO.Unbind();
	m_VBO.Unbind();
	m_EBO.Unbind();
}


void Mesh::Draw(Shader& shader, glm::mat4 model) {  //use my default shaders and setup view model projection for testing
	shader.Activate();
	shader.SetMat4("u_Model", model);
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;

	// Bind textures if any
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		//glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// Optionally set sampler uniform if needed
		// shader.SetInt("texture" + std::to_string(i), i);

		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

			shader.SetInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0); // Reset active texture


    m_VAO.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	m_VAO.Unbind();

	// Unbind textures
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}

