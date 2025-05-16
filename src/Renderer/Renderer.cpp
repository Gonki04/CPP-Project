#include "Renderer.h"

namespace Render {

	Renderer::Renderer()
	{
		// Initialize the shader
		
	}

	void Renderer::Load(std::string obj_model_filepath) //Load .obj file
	{
		fstream file(obj_model_filepath, ios::in);
		if (!file.is_open())
		{
			std::cerr << "Could not open the file!" << std::endl;
			return;
		}
		std::string line;
		if (file.is_open())
		{
			while (getline(file, line))
			{
				std::istringstream iss(line);
				std::string prefix;
				iss >> prefix;
				if (prefix == "v") //take into account the Vertex struct
				{
					Vertex vertex;
					iss >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z;
					iss >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
					iss >> vertex.TexCoords.x >> vertex.TexCoords.y;
					vertices.push_back(vertex);
				}
				else if (prefix == "vt") //take into account the Texture struct
				{
					Texture texture;
					iss >> texture.id >> texture.type;
					textures.push_back(texture);
				}
				else if (prefix == "vn")
				{
					glm::vec3 normal;
					iss >> normal.x >> normal.y >> normal.z;
				}
				else if (prefix == "f")
				{
					unsigned int index[3];
					iss >> index[0] >> index[1] >> index[2];
					indices.push_back(index[0] - 1);
					indices.push_back(index[1] - 1);
					indices.push_back(index[2] - 1);
				}
			}
			file.close();
		}
		else
		{
			std::cerr << "Could not open the file!" << std::endl;
		}

	}

	void Renderer::Install(void)
	{
		meshes.push_back(Mesh(vertices, indices, textures));
		meshes[0].setupMesh();
	}

	void Renderer::Render(glm::vec3 position, glm::vec3 orientation)
	{
		for (auto& mesh : meshes)
		{
			mesh.Draw(shader);
		}
	}

}