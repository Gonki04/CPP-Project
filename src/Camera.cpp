#include <Camera.h>

Camera::Camera(int width, int height, glm::vec3 position) : WIDTH(width), HEIGHT(height) {
    ViewMatrix = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

// void Camera::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
//     if (yoffset == 1) {
//         ZOOM += fabs(ZOOM) * 0.1f;
//     } else if (yoffset == -1) {
//         ZOOM -= fabs(ZOOM) * 0.1f;
//     }
//     std::cout << "ZOOM = " << ZOOM << std::endl;
// }