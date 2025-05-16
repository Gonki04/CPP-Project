#include "../headers.h"
#include "VBO.h"

	class VAO
	{
	public:
		GLuint ID;
		// CONSTRUCTOR QUE CRIA O VAO
		VAO();
		// BINDING DO VAO
		void Bind();
		// UNBINDING DO VAO
		void Unbind();
		// DELETAR O VAO
		void Delete();
	};
