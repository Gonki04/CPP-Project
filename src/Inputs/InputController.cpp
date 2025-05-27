#include "InputController.h"
#include <iostream>

namespace Render
{
    InputController::InputController(Camera *camera)
        : camera(camera)
    {
        modelYaw = -90.0f;
        modelPitch = 0.0f;
        lastX = 400.0f;
        lastY = 300.0f;
        firstMouse = true;
    }

    void InputController::SetTableMesh(Mesh *table_Mesh)
    {
        this->table_Mesh = table_Mesh;
        if (target)
            delete target;
        if (table_Mesh)
            target = new glm::vec3(table_Mesh->GetCenter());
        else
            target = nullptr;
    }

        // Add this setter for Balls
    void InputController::SetBalls(Balls *balls_ptr)
    {
        this->balls = balls_ptr;
    }


    void InputController::SetCallbacks(GLFWwindow *window)
    {
        glfwSetWindowUserPointer(window, this);

        glfwSetCursorPosCallback(window, [](GLFWwindow *win, double xpos, double ypos)
                                 {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->CursorCallback(win, xpos, ypos); });

        glfwSetScrollCallback(window, [](GLFWwindow *win, double xoffset, double yoffset)
                              {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->ScrollCallback(win, xoffset, yoffset); });

        glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods)
                           {
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->KeyCallback(win, key, scancode, action, mods); });
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
        std::cout << "Camera Orientation: " << camera->Orientation.x << ", " << camera->Orientation.y << ", " << camera->Orientation.z << std::endl;
    }

    glm::mat4 InputController::GetGlobalRotationMatrix() const
    {
        glm::mat4 rotation = glm::mat4(1.0f);
        // Rotate around the table's center if available, otherwise origin
        glm::vec3 rotationCenter = target ? *target : glm::vec3(0.0f);

        // Translate to origin, rotate, then translate back
        rotation = glm::translate(rotation, rotationCenter);
        rotation = glm::rotate(rotation, glm::radians(modelYaw), glm::vec3(0.0f, 1.0f, 0.0f));   // Yaw around Y-axis
        rotation = glm::rotate(rotation, glm::radians(modelPitch), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch around X-axis
        rotation = glm::translate(rotation, -rotationCenter);

        return rotation;
    }

    void InputController::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {

        if (key == GLFW_KEY_1)
        {
            if (action == GLFW_PRESS && prev1 == GLFW_RELEASE)
            {
                ambientEnabled = !ambientEnabled;
            }
            prev1 = action;
        }

        if (key == GLFW_KEY_2)
        {
            if (action == GLFW_PRESS && prev2 == GLFW_RELEASE)
            {
                directionalEnabled = !directionalEnabled;
            }
            prev2 = action;
        }

        if (key == GLFW_KEY_3)
        {
            if (action == GLFW_PRESS && prev3 == GLFW_RELEASE)
            {
                pointEnabled = !pointEnabled;
            }
            prev3 = action;
        }

        if (key == GLFW_KEY_4)
        {
            if (action == GLFW_PRESS && prev4 == GLFW_RELEASE)
            {
                spotEnabled = !spotEnabled;
            }
            prev4 = action;
        }

        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            // define a posiçao da bola 1 para uma nova posiçao
            balls->ResetBall(1, glm::vec3(-20.0f, 4.0f, 0.0f));
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            // define o speed da bola 0 para 10.0f
            balls->SetBallSpeed(0, 10.0f);
        }
    }
}