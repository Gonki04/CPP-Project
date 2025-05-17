#ifndef EBO_CLASS_H
#define EBO_CLASS_H
#pragma once

#include <GL/glew.h>
#include <vector>

class EBO
{
public:
	GLuint ID;
	EBO(); // <-- Add this line if you define EBO::EBO() in EBO.cpp
	~EBO();

	void BufferData(const std::vector<GLuint> &indices);
	void Bind();
	void Unbind();
};

#endif