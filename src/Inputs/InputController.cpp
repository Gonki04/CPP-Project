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

    // Funçao que regista os callbacks de input na janela GLFW
    void InputController::SetCallbacks(GLFWwindow *window)
    {
        glfwSetWindowUserPointer(window, this); // Armazena o ponteiro do InputController na janela GLFW

        glfwSetCursorPosCallback(window, [](GLFWwindow *win, double xpos, double ypos)
                                 { // Callback para o movimento do rato
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->CursorCallback(win, xpos, ypos); }); 

        glfwSetScrollCallback(window, [](GLFWwindow *win, double xoffset, double yoffset)
                              { // Callback para o scroll do rato
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->ScrollCallback(win, xoffset, yoffset); });

        glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods)
                           { // Callback para o teclado
        auto* controller = static_cast<InputController*>(glfwGetWindowUserPointer(win));
        if (controller) controller->KeyCallback(win, key, scancode, action, mods); });
    }

    // Função que lida com o scroll do rato para alterar o FOV (zoom in/out)
    void InputController::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        camera->HandleScroll(yoffset); // Chama a função da câmara para o zoom in/out
    }

    // Função que lida com o movimento do rato para controlar a rotação do modelo
    void InputController::CursorCallback(GLFWwindow *window, double xpos, double ypos)
    {
        if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS)) // Verificação se o botão esquerdo do rato é pressionado
        {
            return;
        }

        if (firstMouse) // Verifica se é o primeiro movimento do rato
        {
            lastX = xpos; // Armazena a posição X do rato
            lastY = ypos; // Armazena a posição Y do rato
            firstMouse = false; 
        }

        float xoffset = xpos - lastX; // Diferença entre a posição atual e a última posição X do rato
        float yoffset = lastY - ypos; // Diferença entre a posição atual e a última posição Y do rato
        lastX = xpos; // Atualiza a última posição X do rato
        lastY = ypos; // Atualiza a última posição Y do rato

        float sensitivity = 0.1f; // Sensibilidade do rato
        xoffset *= sensitivity; // Multiplica a diferença X pela sensibilidade
        yoffset *= sensitivity; // Multiplica a diferença Y pela sensibilidade

        modelYaw += xoffset; // Atualiza o ângulo de rotação em Y com a diferença X
        modelPitch += yoffset; // Atualiza o ângulo de rotação em X com a diferença Y

        if (modelPitch > 89.0f) // Limita o ângulo de rotação em X para não ultrapassar 89 graus 
            modelPitch = 89.0f; 
        if (modelPitch < -89.0f) // Limita o ângulo de rotação em X para não ultrapassar -89 graus
            modelPitch = -89.0f;

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

    // Função que lida com as teclas pressionadas para ativar/desativar as luzes e controlar as bolas
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