#include "InputController.h"
#include <iostream>

InputController::InputController(Camera *camera)
    : camera(camera)
{
    modelYaw = -90.0f;
    modelPitch = 0.0f;
    lastX = 400.0f;
    lastY = 300.0f;
    firstMouse = true;
}

void InputController::SetTableMesh(Mesh* table_Mesh)
{
    this->table_Mesh = table_Mesh;
    if (target) delete target;
    if (table_Mesh)
        target = new glm::vec3(table_Mesh->GetCenter());
    else
        target = nullptr;
}

void InputController::SetCallbacks(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->CursorCallback(win, xpos, ypos);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->ScrollCallback(win, xoffset, yoffset);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->KeyCallback(win, key, scancode, action, mods);
    });
}

void InputController::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera->HandleScroll(yoffset);
}

void InputController::CursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS))
    {
        return;
    }

    if (firstMouse)
    {
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

    modelYaw += xoffset;
    modelPitch += yoffset;

    if (modelPitch > 89.0f)
        modelPitch = 89.0f;
    if (modelPitch < -89.0f)
        modelPitch = -89.0f;

    std::cout << "Yaw: " << modelYaw << ", Pitch: " << modelPitch << std::endl;
    std::cout << "Camera Position: " << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z << std::endl;
}

void InputController::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {

    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {

    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {

    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        /* code */
    }
    
}