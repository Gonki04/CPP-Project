#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include"VBO.h"

class VAO
{
public:
	GLuint ID;
	VAO();

	//Linka o atributo VBO
	void LinkVBO(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif