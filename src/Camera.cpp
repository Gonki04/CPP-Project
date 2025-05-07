#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : zoom_(position.z)
{
    view_matrix_ = glm::lookAt(position, glm::vec3(0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::HandleScroll(double yoffset)
{
    if (yoffset == 1)
    {
        zoom_ += fabs(zoom_) * 0.1f;// Zoom in
    }
    else if (yoffset == -1)
    {
        zoom_ -= fabs(zoom_) * 0.1f;// Zoom out
    }
    zoom_ = std::max(0.1f, zoom_); // Prevent zoom_ <= 0
    std::cout << "Zoom: " << zoom_ << std::endl;
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        camera->HandleScroll(yoffset);
    }
}