#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(fov_), (float)width / (float)height, nearPlane, farPlane);
    glm::mat4 viewProjection = projection * view;

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Camera::HandleScroll(double yoffset) {
    if (fov_ >= 1.0f && fov_ <= 45.0f)
        fov_ -= yoffset;
    if (fov_ <= 1.0f)
        fov_ = 1.0f;
    if (fov_ >= 45.0f)
        fov_ = 45.0f;
}


void Camera::HandleKeyboardInput(GLFWwindow *window, float deltaSpeed) {
    const float cameraSpeed = 10.5f * deltaSpeed; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += cameraSpeed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= cameraSpeed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
}

void Camera::ProcessMouseRotation(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static double lastX = 0.0;
    static double lastY = 0.0;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos); // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity * 0.001f;
    yoffset *= sensitivity * 0.001f;

    static float yaw = -90.0f;   // Yaw is initialized to -90.0 degrees to look towards -Z
    static float pitch = 0.0f;

    yaw += xoffset;
    pitch += yoffset;

    // Clamp pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Update orientation vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Orientation = glm::normalize(direction);
}

