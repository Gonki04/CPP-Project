#include "Mesh.h"
#include "../../Texture/Texture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(Shader &shader, std::string obj_model_filepath)
{
	this->shader = shader;
	Load(obj_model_filepath);
	Install();
}



void Mesh::Draw(Shader &shader, glm::mat4 model)
{ // use my default shaders and setup view model projection for testing
	shader.SetMat4("u_Model", model);

	shader.SetVec3("material.ambient", material.ambient);
    shader.SetVec3("material.diffuse", material.diffuse);
    shader.SetVec3("material.specular", material.specular);
    shader.SetFloat("material.shininess", material.shininess);
std::cout << "[" << name << "] Material ambient: " << material.ambient.x << ", " << material.ambient.y << ", " << material.ambient.z << std::endl;
std::cout << "[" << name << "] Material diffuse: " << material.diffuse.x << ", " << material.diffuse.y << ", " << material.diffuse.z << std::endl;
std::cout << "[" << name << "] Material specular: " << material.specular.x << ", " << material.specular.y << ", " << material.specular.z << std::endl;
std::cout << "[" << name << "] Material shininess: " << material.shininess << std::endl;
	// Bind textures if any
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// Optionally set sampler uniform if needed
		// shader.SetInt("texture" + std::to_string(i), i);
	}
	glActiveTexture(GL_TEXTURE0); // Reset active texture

	m_VAO.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	m_VAO.Unbind();

	// Unbind textures
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Load(std::string obj_model_filepath)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::map<std::tuple<int, int, int>, unsigned int> uniqueVertexMap;
	std::fstream file(obj_model_filepath, std::ios::in);
	std::map<std::string, Material> materials;
	std::string currentMaterialName;
	std::string mtlFilePath;
	if (!file.is_open())
	{
		std::cerr << "Could not open the file!" << std::endl;
		return;
	}
	std::string line;
	while (getline(file, line))
	{
		// Skip empty lines or comments
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v")
		{
			glm::vec3 pos;
			iss >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (prefix == "vt")
		{
			glm::vec2 tex;
			iss >> tex.x >> tex.y;
			texcoords.push_back(tex);
		}
		else if (prefix == "vn")
		{
			glm::vec3 norm;
			iss >> norm.x >> norm.y >> norm.z;
			normals.push_back(norm);
		}
		else if (prefix == "f")
		{
			std::string vertexStr;
			for (int i = 0; i < 3; ++i)
			{
				iss >> vertexStr;
				size_t pos1 = vertexStr.find('/');
				size_t pos2 = vertexStr.find('/', pos1 + 1);

				int vIdx = std::stoi(vertexStr.substr(0, pos1)) - 1;
				int vtIdx = std::stoi(vertexStr.substr(pos1 + 1, pos2 - pos1 - 1)) - 1;
				int vnIdx = std::stoi(vertexStr.substr(pos2 + 1)) - 1;

				// Check ranges before accessing
				if (vIdx < 0 || vIdx >= positions.size() ||
					vtIdx < 0 || vtIdx >= texcoords.size() ||
					vnIdx < 0 || vnIdx >= normals.size())
				{
					std::cerr << "OBJ index out of range: v=" << vIdx << " vt=" << vtIdx << " vn=" << vnIdx << std::endl;
					continue; // or handle error
				}

				auto key = std::make_tuple(vIdx, vtIdx, vnIdx);
				if (uniqueVertexMap.count(key) == 0)
				{
					Vertex vert;
					vert.Position = positions[vIdx];
					vert.TexCoords = texcoords[vtIdx];
					vert.Normal = normals[vnIdx];
					vertices.push_back(vert);
					uniqueVertexMap[key] = static_cast<unsigned int>(vertices.size() - 1);
				}
				indices.push_back(uniqueVertexMap[key]);
			}
		}
		else if (prefix == "mtllib") // Material library
		{
			iss >> mtlFilePath;
			mtlFilePath = Texture::GetTexturePath(obj_model_filepath, mtlFilePath);
			Texture::LoadMTL(mtlFilePath, materials);
		}
		else if (prefix == "usemtl")
		{
			iss >> currentMaterialName;
			if (materials.find(currentMaterialName) != materials.end())
			{
				this->material = materials[currentMaterialName]; // Store in member variable

				if (material.diffuseMap != 0)
				{
					Texture texture;
					texture.id = material.diffuseMap;
					texture.type = "texture_diffuse";
					textures.push_back(texture);
				}
			}
		}
	}
	file.close();

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
}

glm::vec3 Mesh::GetCenter()
{
	glm::vec3 center(0.0f);
	for (const auto &vertex : vertices)
	{
		center += vertex.Position;
	}
	center /= static_cast<float>(vertices.size());
	return center;
}

void Mesh::Install(void)
{
	if (vertices.empty() || indices.empty())
	{
		std::cerr << "Mesh::setupMesh: vertices or indices are empty!" << std::endl;
		return;
	}

	m_VAO.Bind();
	m_VBO.Bind();
	m_EBO.Bind();

	m_VBO.BufferData(vertices);
	m_EBO.BufferData(indices);

	m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
	m_VAO.LinkVBO(m_VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

	m_VAO.Unbind();
	m_VBO.Unbind();
	m_EBO.Unbind();
}

void Mesh::Render(glm::vec3 position, glm::vec3 orientation)
{

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	transform = glm::rotate(transform, glm::radians(orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Draw(shader, transform);
}