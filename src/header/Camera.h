#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw/glfw3.h>

#include <iostream>
#include <vector>

class Camera {
    public:
            int WIDTH, HEIGHT;
            GLfloat ZOOM = 10.0f;
            GLfloat ANGLE = 0.0f;

            glm::mat4 ViewMatrix = glm::lookAt(
                glm::vec3(0.0f, 0.0f, ZOOM),
                glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 model = glm::mat4(1.0f);
            
            Camera(int width, int height, glm::vec3 position);
            //void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};