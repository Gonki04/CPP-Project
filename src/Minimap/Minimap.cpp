#include "Minimap.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Renderer.h"
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Balls/Balls.h"

namespace Render {

void drawMinimap(Mesh& tableMesh, Balls& balls, Shader* shader, int windowWidth, int windowHeight) {
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    
    int minimapWidth = 200;
    int minimapHeight = 150;
    glViewport(windowWidth - minimapWidth - 10, windowHeight - minimapHeight - 10,
               minimapWidth, minimapHeight);

    
    shader->Activate();

    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 20.0f, 0.0f),  
        glm::vec3(0.0f, 0.0f, 0.0f),   
        glm::vec3(0.0f, 0.0f, -1.0f)   
    );

    
    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, 0.1f, 50.0f);

    
    shader->SetMat4("u_Model", model);
    shader->SetMat4("u_View", view);
    shader->SetMat4("u_Projection", projection);

    
    tableMesh.Render(glm::vec3(0.0f), glm::vec3(0.0f), model);

    
    balls.DrawPoolBalls(model);

    
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

}