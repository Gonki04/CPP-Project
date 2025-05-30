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

        lights[0] = new Light(LightType::Ambient);
        lights[1] = new Light(LightType::Directional);
        lights[2] = new Light(LightType::Point);
        lights[3] = new Light(LightType::Spot);
    }

    InputController::~InputController()
    {
        for (int i = 0; i < 4; i++) {
            delete lights[i];
        }
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
        float yoffset = lastY - ypos;
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

    }

    glm::mat4 InputController::GetGlobalRotationMatrix() const
    {
        glm::mat4 rotation = glm::mat4(1.0f);

        glm::vec3 rotationCenter = target ? *target : glm::vec3(0.0f);


        rotation = glm::translate(rotation, rotationCenter);
        rotation = glm::rotate(rotation, glm::radians(modelYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        
        rotation = glm::translate(rotation, -rotationCenter);

        return rotation;
    }

    void InputController::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            lights[0]->Enable(!lights[0]->enabled);
        }

        if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            lights[1]->Enable(!lights[1]->enabled);
        }

        if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        {
            lights[2]->Enable(!lights[2]->enabled);
        }

        if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        {
            lights[3]->Enable(!lights[3]->enabled);
        }

        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            balls->ResetBall();
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            balls->SetBallSpeed(0, 10.0f);
        }
    }
}
