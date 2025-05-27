#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera/Camera.h"
#include "../Renderer/Mesh/Mesh.h"
#include "../Renderer/Balls.h"

namespace Render
{
    class Balls;
    class InputController // Classe para controlar inputs do utilizador
    {
    public:
        InputController(Camera *camera); // Construtor
        ~InputController(); // Destrutor

        void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset); // Callback para o scroll do rato
        void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods); // Callback para o teclado
        void CursorCallback(GLFWwindow *window, double xpos, double ypos); // Callback para o movimento do rato
        void SetCallbacks(GLFWwindow *window); // Regista os callbacks na janela GLFW
        void SetTableMesh(Mesh *table_Mesh); // Define a mesh da mesa
        void SetBalls(Balls *balls_ptr); // Define o ponteiro para as bolas

        float modelYaw; // Ângulo de rotação em Y
        float modelPitch; // Ângulo de rotação em X

        bool ambientEnabled = true; // Luz ambiente ativa
        bool directionalEnabled = true; // Luz direcional ativa
        bool pointEnabled = true; // Luz pontual ativa
        bool spotEnabled = true; // Luz spot ativa

        glm::mat4 GetGlobalRotationMatrix() const; // Retorna a matriz de rotação global

    private:
        Camera *camera; // Ponteiro para a câmara
        Mesh *table_Mesh = nullptr;  // Ponteiro para a mesh da mesa
        glm::vec3 *target = nullptr; // Centro da mesa
        Balls *balls = nullptr; // Ponteiro para as bolas

        int prev1 = GLFW_RELEASE, prev2 = GLFW_RELEASE, prev3 = GLFW_RELEASE, prev4 = GLFW_RELEASE; // Estados anteriores das teclas

        float lastX, lastY; // Últimas posições do rato
        bool firstMouse; // Indica se é o primeiro movimento do rato
    };
}