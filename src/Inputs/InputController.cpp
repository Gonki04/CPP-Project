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
            // Reseta a posição e velocidade das bolas
            balls->ResetBall();
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            // define o speed da bola 0 para 10.0f
            balls->SetBallSpeed(0, 10.0f);
        }
    }
}
