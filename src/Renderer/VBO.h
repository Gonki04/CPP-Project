#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace Render {

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class VBO {
public:
	VBO();
	GLuint ID;
	void BufferData(const std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};

}