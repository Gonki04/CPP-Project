#include "../headers.h"



	class VBO
	{
	public:
		GLuint ID;
		// CONSTRUCTOR QUE CRIA O VBO

		VBO(GLfloat* vertices, GLsizeiptr size);
		// BINDING DO VBO
		void Bind();
		// UNBINDING DO VBO
		void Unbind();
		// DELETAR O VBO
		void Delete();
	};
