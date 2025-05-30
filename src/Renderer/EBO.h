#pragma once

#include <GL/glew.h>
#include <vector>

namespace Render
{
	class EBO
	{
	public:
		GLuint ID; 
		EBO(); 
		~EBO(); 

		void BufferData(const std::vector<GLuint> &indices); 
		void Bind(); 
		void Unbind(); 
	};

}