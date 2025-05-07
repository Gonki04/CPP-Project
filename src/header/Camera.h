#ifndef SRC_HEADER_CAMERA_H_
#define SRC_HEADER_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfw/glfw3.h>

#include <iostream>
#include <vector>

class Camera
{
public:
    Camera(int width, int height, glm::vec3 position);

    // Método para lidar com eventos de scroll
    void HandleScroll(double yoffset);

    float zoom() const { return zoom_; }
    float fov() const { return fov_; } // Getter for FOV
    glm::mat4 view_matrix() const { return view_matrix_; }
    glm::mat4 model() const { return model_; }

private:
    float zoom_;
    float fov_; // Field of view
    glm::mat4 view_matrix_;
    glm::mat4 model_;
};

// Standalone C-style function (forward declaration)
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

#endif // SRC_HEADER_CAMERA_H_