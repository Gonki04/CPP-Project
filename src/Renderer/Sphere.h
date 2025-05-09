#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Mesh/Mesh.h"

class Sphere {
public:
    Sphere(float radius, int sectorCount, int stackCount);
    ~Sphere();

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

    void Draw();
    void GetVerticesAndIndices(std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);

private:
    float radius;
    int sectorCount;
    int stackCount;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh* mesh;

    void generateSphere();
};