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
	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind Vertex Array Object
	glBindVertexArray(VAO);
	// Bind and set vertex buffer(s)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	// Bind and set element buffer(s)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// Set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	// Texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);
	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);

}


/*default.vert
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoord;

void main() {
	FragPos = vec3(u_Model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	TexCoord = aTexCoord;
	Color = aColor;

	gl_Position = u_View * u_Projection * u_Model * vec4(FragPos, 1.0);
}

default.frag

#version 450 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

void main() {
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0);
	vec3 ambient = vec3(0.1);

	FragColor = vec4((ambient + diffuse) * Color, 1.0);
}
*/

void Mesh::Draw(Shader& shader) {  //use my default shaders and setup view model projection for testing
	shader.Activate();
	shader.SetMat4("u_Model", glm::mat4(1.0f));
	shader.SetMat4("u_View", glm::mat4(1.0f));
	shader.SetMat4("u_Projection", glm::mat4(1.0f));

	// Bind textures if any
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// Optionally set sampler uniform if needed
		// shader.SetInt("texture" + std::to_string(i), i);
	}
	glActiveTexture(GL_TEXTURE0); // Reset active texture

	// Draw mesh
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Unbind textures
	for (unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}