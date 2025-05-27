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
	Mesh(Shader &shader, std::string obj_model_filepathe);
	glm::vec3 GetCenter();
	void Render(glm::vec3 position, glm::vec3 orientation);
	glm::vec3 velocity;
	std::string name;


private:
	void Load(std::string obj_model_filepath);
	void Install();
	void Draw(Shader &shader, glm::mat4 model = glm::mat4(1.0f));
	Material material;
	std::map<std::string, Material> materials;
	VAO m_VAO;
	VBO m_VBO;
	EBO m_EBO;
	Shader shader;
	std::vector<Mesh> meshes;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};
}