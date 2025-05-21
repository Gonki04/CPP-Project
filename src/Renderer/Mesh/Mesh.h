#pragma once
#include "../../headers.h"
#include "../Shader/shaderClass.h"
#include "../VBO.h"
#include "../EBO.h"
#include "../VAO.h"
#include "../../Texture/Texture.h"

// struct Texture {
// 	unsigned int id;
// 	std::string type;
// };

class Mesh
{
public:
	

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void setupMesh();

	void Draw(Shader& shader, glm::mat4 model = glm::mat4(1.0f));
		

private:
	VAO m_VAO;
    VBO m_VBO;
    EBO m_EBO;
    std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

};