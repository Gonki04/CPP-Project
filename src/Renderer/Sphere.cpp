#include "Sphere.h"
#include <cmath>

Sphere::Sphere(float radius, int sectorCount, int stackCount)
    : radius(radius), sectorCount(sectorCount), stackCount(stackCount)
{
    generateSphere();
}

void Sphere::generateSphere()
{
    const float PI = 3.14159265359f;

    // Gerar vértices
    for (int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = PI / 2 - i * (PI / stackCount); // De +PI/2 a -PI/2
        float xy = radius * cos(stackAngle);               // Raio no plano XY
        float z = radius * sin(stackAngle);                // Coordenada Z

        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * (2 * PI / sectorCount); // De 0 a 2PI

            // Coordenadas do vértice
            float x = xy * cos(sectorAngle);
            float y = xy * sin(sectorAngle);

            // Normal do vértice
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            // Coordenadas de textura (mapeamento esférico)
            float u = (float)j / sectorCount; // Longitude normalizada [0, 1]
            float v = (float)i / stackCount;  // Latitude normalizada [0, 1]

            // Cor do vértice (opcional, aqui usamos a normal como cor)
            glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

            // Adicionar vértice
            //vertices.push_back({glm::vec3(x, y, z), normal, color, glm::vec2(u, v)});
        }
    }

    // Gerar índices
    for (int i = 0; i < stackCount; ++i)
    {
        int k1 = i * (sectorCount + 1); // Início da stack atual
        int k2 = k1 + sectorCount + 1;  // Início da próxima stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                // Triângulo superior
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1))
            {
                // Triângulo inferior
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::GetVerticesAndIndices(std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
{
    outVertices = vertices;
    outIndices = indices;
}
