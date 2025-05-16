#include "../headers.h"


	class EBO
	{
	public:
		GLuint ID;
		// CONSTRUCTOR QUE CRIA O EBO
		EBO(GLuint* indices, GLsizeiptr size);
		// BINDING DO EBO
		void Bind();
		// UNBINDING DO EBO
		void Unbind();
		// DELETAR O EBO
		void Delete();
	};
