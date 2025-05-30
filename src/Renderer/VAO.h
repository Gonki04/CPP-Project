#pragma once

#include "VBO.h"

namespace Render
{
	class VAO
	{
	public:
		GLuint ID; 
		VAO(); 

		void LinkVBO(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset); 
		void Bind(); 
		void Unbind(); 
		void Delete(); 
	};

}