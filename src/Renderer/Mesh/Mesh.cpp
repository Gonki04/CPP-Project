#include "Mesh.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ourSpace
{
    bool Mesh::Load(const std::string &obj_model_filepath)
    {
        std::ifstream file(obj_model_filepath);
        if (!file.is_open())
        {
            std::cerr << "Erro ao abrir o ficheiro: " << obj_model_filepath << std::endl;
            return false;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        m_Vertices.clear();
        m_Indices.clear();

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
            }
            else if (prefix == "vn")
            {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "vt")
            {
                glm::vec2 tex;
                iss >> tex.x >> tex.y;
                texCoords.push_back(tex);
            }
            else if (prefix == "f")
            {
                unsigned int vIdx[3], tIdx[3], nIdx[3];
                char slash;
                for (int i = 0; i < 3; ++i)
                {
                    iss >> vIdx[i] >> slash >> tIdx[i] >> slash >> nIdx[i];
                    Vertex vertex;
                    vertex.position = positions[vIdx[i] - 1];
                    vertex.normal = normals[nIdx[i] - 1];
                    vertex.texCords = texCoords[tIdx[i] - 1];
                    m_Vertices.push_back(vertex);
                    m_Indices.push_back(static_cast<GLuint>(m_Vertices.size() - 1));
                }
            }
        }

        m_IndexCount = static_cast<GLsizei>(m_Indices.size());
        return true;
    }

    void Mesh::Install()
    {
        m_VAO.Bind();

        m_VBO.Bind();
        m_VBO.BufferData(m_Vertices); // envia para GPU

        m_EBO.Bind();
        m_EBO.BufferData(m_Indices); // envia para GPU

        // Configurar atributos dos vértices (posição, normal, UV)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0); // posição
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal)); // normal
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCords)); // UV
        glEnableVertexAttribArray(2);

        m_VAO.Unbind();
    }
}