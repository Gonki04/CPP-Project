#include "VAO.h"


	VAO::VAO()
	{
		glGenVertexArrays(1, &ID);
	}
	void VAO::Bind()
	{
		glBindVertexArray(ID);
	}
	void VAO::Unbind()
	{
		glBindVertexArray(0);
	}
	void VAO::Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}
