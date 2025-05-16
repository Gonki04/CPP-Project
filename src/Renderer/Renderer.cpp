#include "Renderer.h"

namespace Render
{

    Renderer::Renderer()
    {
        // Initialize the shader
    }

    void Renderer::Load(std::string obj_model_filepath) // Load .obj file
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::map<std::tuple<int, int, int>, unsigned int> uniqueVertexMap;
        fstream file(obj_model_filepath, ios::in);
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
            else if (prefix == "usemtl") // Material
            {
                std::string materialName;
                iss >> materialName;
                // Optionally store material name if needed
            }
            else if (prefix == "mtllib") // Material library
            {
                std::string mtlFileName;
                iss >> mtlFileName;
                // Optionally load material library if needed
            }
        }
        file.close();

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }

    void Renderer::Install(void)
    {
        meshes.push_back(Mesh(vertices, indices, textures));
        meshes[0].setupMesh();
    }

    void Renderer::Render(glm::vec3 position, glm::vec3 orientation)
    {
        for (auto &mesh : meshes)
        {
            mesh.Draw(shader);
        }
    }

}