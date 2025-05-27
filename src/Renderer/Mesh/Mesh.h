#pragma once
#include "../../headers.h"
#include "../Shader/shaderClass.h"
#include "../VBO.h"
#include "../EBO.h"
#include "../VAO.h"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader/shaderClass.h"
#include "../../Texture/Texture.h"

// struct Texture
//{
// 	unsigned int id;
// 	std::string type;
// };
namespace Render{
class Mesh
{
public:
	Mesh(Shader &shader, std::string obj_model_filepathe); // Construtor que carrega o modelo a partir de um ficheiro .obj
	glm::vec3 GetCenter(); // Função que retorna o centro do modelo
	void Render(glm::vec3 position, glm::vec3 orientation,const glm::mat4& globalTransform); // Função que renderiza o modelo na posição e orientação especificadas
	glm::vec3 velocity; // Velocidade do modelo, usada para animações ou movimentos
	std::string name; // Nome do modelo, usado para identificação


private:
	void Load(std::string obj_model_filepath); // Função que carrega o modelo a partir de um ficheiro .obj
	void Install(); // Função que instala o modelo no VAO, VBO e EBO
	void Draw(Shader &shader, glm::mat4 model = glm::mat4(1.0f)); // Função que desenha o modelo usando o shader especificado e a matriz de transformação
	Material material; // Material do modelo, que contém propriedades como cor ambiente, difusa, especular e textura
	std::map<std::string, Material> materials; // Mapa de materiais carregados do ficheiro MTL
	VAO m_VAO; // Vertex Array Object que armazena os atributos dos vértices
	VBO m_VBO; // Vertex Buffer Object que armazena os dados dos vértices
	EBO m_EBO; // Element Buffer Object que armazena os índices dos vértices
	Shader shader; // Shader usado para renderizar o modelo
	std::vector<Mesh> meshes; // Vetor de meshes, usado para armazenar sub-meshes do modelo
	std::vector<Vertex> vertices; // Vetor de vértices que contém as posições, normais e coordenadas de textura
	std::vector<unsigned int> indices; // Vetor de índices para o Element Buffer Object
	std::vector<Texture> textures; // Vetor de texturas a serem aplicadas ao modelo
};
}