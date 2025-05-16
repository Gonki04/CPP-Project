#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCords;
};

class VBO {
public:
	GLuint ID;
	void BufferData(const std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
};



#endif
