//#include "Camera/Camera.h"
//#include <algorithm>
//
//Camera::Camera(int width, int height, glm::vec3 position)
//    : zoom_(position.z), fov_(45.0f) // Initialize FOV to 45 degrees
//{
//    // Set the initial position of the camera above the table
//    view_matrix_ = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f),
//                               glm::vec3(0.0f, 1.0f, 0.0f));
//}
//
//void Camera::HandleScroll(double yoffset)
//{
//    if (yoffset == 1)
//    {
//        fov_ -= 2.0f; // Zoom in by decreasing FOV
//    }
//    else if (yoffset == -1)
//    {
//        fov_ += 2.0f; // Zoom out by increasing FOV
//    }
//
//    // Clamp FOV to a reasonable range (e.g., 1 to 90 degrees)
//    fov_ = std::clamp(fov_, 1.0f, 90.0f);
//
//    std::cout << "FOV: " << fov_ << std::endl;
//}
//
//void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
//{
//    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
//    if (camera)
//    {
//        camera->HandleScroll(yoffset);
//    }
//}