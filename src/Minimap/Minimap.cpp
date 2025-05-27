#include "Minimap.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Renderer.h"
#include "Renderer/Mesh/Mesh.h"


namespace Render{

void drawMinimap(Mesh& tableMesh,Mesh& sphereMesh, Shader* shader, int windowWidth, int windowHeight) {
    // Salvar o viewport atual
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Definir viewport do minimapa
    int minimapWidth = 200;
    int minimapHeight = 150;
    glViewport(windowWidth - minimapWidth - 10, windowHeight - minimapHeight - 10,
               minimapWidth, minimapHeight);

    shader->Activate();

    // Definir as matrizesminimapa
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 0.0f),  // Câmera acima da mesa
        glm::vec3(0.0f, 0.0f, 0.0f),  // Apontar para o centro da mesa
        glm::vec3(0.0f, 0.0f, -1.0f)  // Up invertido para visão top-down
    );
    glm::mat4 projection = glm::ortho(-2.5f, 2.5f, -1.25f, 1.25f, 0.1f, 10.0f);

    shader->SetMat4("u_Model", model);
    shader->SetMat4("u_View", view);
    shader->SetMat4("u_Projection", projection);

    // Desenhar a mesh da mesa
    //tableMesh.Draw();
    //sphereMesh.Draw();

    // Restaurar viewport original
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}
}