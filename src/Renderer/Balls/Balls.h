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

        void BallsControl(GLFWwindow *window, double deltaTime, glm::mat4 &globalRotationMatrix);
        void ResetBall();
        void SetBallSpeed(int index, double newSpeed);

    private:
        Shader &shader;
        std::vector<Mesh> poolBalls;
        std::vector<glm::vec3> ballPositions;
        std::vector<glm::vec3> ballOrientations;

        void GeneratePoolBalls(Shader &shader);
        void DrawPoolBalls(glm::mat4 &globalRotationMatrix);
        void AnimateBall(GLFWwindow *window, double deltaTime);
        void DetectBallsCollisions();
        void CalculateTableBorders();
        void BallsRotation();
    };
}