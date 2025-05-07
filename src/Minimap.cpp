#include "Minimap.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Forward declaration of display (or move display to a shared file if needed)
void display(std::vector<glm::vec3> points, glm::mat4 mvp);

void drawMinimap(const std::vector<glm::vec3>& points, int windowWidth, int windowHeight) {
    int minimapWidth = 200;
    int minimapHeight = 150;
    glViewport(10, windowHeight - minimapHeight - 10, minimapWidth, minimapHeight);

    glm::mat4 projection = glm::ortho(-3.0f, 3.0f, -2.0f, 2.0f, -10.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    );
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;

    display(points, mvp);

    glViewport(0, 0, windowWidth, windowHeight);
}