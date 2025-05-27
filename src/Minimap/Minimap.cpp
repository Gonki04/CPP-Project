#include "Minimap.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Renderer.h"
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Balls/Balls.h"

namespace Render {

void drawMinimap(Mesh& tableMesh, Balls& balls, Shader* shader, int windowWidth, int windowHeight) {
    // Salvar o viewport atual
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Definir viewport do minimapa (canto superior direito)
    int minimapWidth = 200;
    int minimapHeight = 150;
    glViewport(windowWidth - minimapWidth - 10, windowHeight - minimapHeight - 10,
               minimapWidth, minimapHeight);

    // Ativar o shader
    shader->Activate();

    // Configurar a câmera do minimapa (visão de cima)
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 20.0f, 0.0f),  // Posição da câmera (mais alta)
        glm::vec3(0.0f, 0.0f, 0.0f),   // Ponto de foco (centro da mesa)
        glm::vec3(0.0f, 0.0f, -1.0f)   // Vetor up invertido para visão top-down
    );

    // Projeção ortográfica para o minimapa
    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, 0.1f, 50.0f);

    // Enviar matrizes para o shader
    shader->SetMat4("u_Model", model);
    shader->SetMat4("u_View", view);
    shader->SetMat4("u_Projection", projection);

    // Desenhar a mesa no minimapa
    tableMesh.Render(glm::vec3(0.0f), glm::vec3(0.0f), model);

    // Desenhar todas as bolas no minimapa
    balls.DrawPoolBalls(model);

    // Restaurar viewport original
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

}