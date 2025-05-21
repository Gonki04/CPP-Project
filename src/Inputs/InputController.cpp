#include "InputController.h"
#include <iostream>

InputController::InputController(Camera* camera, Mesh* table_Mesh)
    : camera(camera), table_Mesh(table_Mesh) {
    // Supondo que table_Mesh->GetCenter() retorna glm::vec3 do centro da mesa
    target = new glm::vec3(table_Mesh->GetCenter());
    yaw = -90.0f;
    pitch = 0.0f;
    lastX = 400.0f;
    lastY = 300.0f;
    firstMouse = true;
    distanceToTarget = glm::length(camera->Position - *target);
}

void InputController::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Zoom in/out with mouse scroll
    camera->HandleScroll(yoffset);
}

void InputController::MouseCallback(GLFWwindow* window, double xpos, double ypos) {

    if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS)){
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // invertido
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calcula nova posição da câmera em torno do centro da mesa
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    camera->Position = *target - direction * distanceToTarget;
    camera->Orientation = glm::normalize(*target - camera->Position);
}