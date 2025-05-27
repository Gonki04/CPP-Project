#pragma once
#include "../../headers.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Renderer.h"

namespace Render
{
    class Balls
    {
    public:
        Balls(Shader &shader)
            : shader(shader)
        {
            GeneratePoolBalls(shader); // assim que a classe for instanciada, as bolas serão geradas
        }

        void BallsControl(GLFWwindow *window, double deltaTime,glm::mat4 &globalRotationMatrix); // controla as bolas, atualizando suas posições e animações
        void ResetBall(); // reseta a posição e velocidade da bola branca
        void SetBallSpeed(int index, double newSpeed); // define a velocidade de uma bola específica

    private:
        Shader &shader; // referência ao shader usado para renderizar as bolas
        std::vector<Mesh> poolBalls; // vetor de meshes que representa as bolas de bilhar
        std::vector<glm::vec3> ballPositions; // posições das bolas de bilhar
        std::vector<glm::vec3> ballOrientations; // orientações das bolas de bilhar

        void GeneratePoolBalls(Shader &shader);
        void DrawPoolBalls(glm::mat4 &globalRotationMatrix);
        void AnimateBall(GLFWwindow *window, double deltaTime);
        void DetectBallsCollisions();
        void CalculateTableBorders();
        void BallsRotation();
    };
}