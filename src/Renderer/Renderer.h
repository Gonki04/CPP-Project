#include "headers.h"
#include "Mesh/Mesh.h"
#include "Shader/shaderClass.h"
#include <tuple>
#include <map>

namespace Render
{
	using namespace std;
	class Renderer {
	public:

		Renderer();

		void Load(const std::string obj_model_filepath);
		void Install(void);
		void Render(glm::vec3 position, glm::vec3 orientation);


	private:
		Shader shader = Shader("resources/default.vert", "resources/default.frag");
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		std::vector<Mesh> meshes;


	};
}