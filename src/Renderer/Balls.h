#pragma once
#include "../headers.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Renderer.h"

class Balls
{
public:
    Balls(Shader &shader)
        : shader(shader)
    {
        GeneratePoolBalls(shader);
    }

    void BallsControl(GLFWwindow *window, double deltaTime);
    void ResetBall(int index, const glm::vec3 &pos);
    void SetBallSpeed(int index, double newSpeed);

private:
    Shader &shader;
    std::vector<Mesh> poolBalls;
    std::vector<glm::vec3> ballPositions;
    std::vector<glm::vec3> ballOrientations;

    void GeneratePoolBalls(Shader &shader);
    void DrawPoolBalls();
    void AnimateBall(GLFWwindow *window, double deltaTime);
    void DetectBallsCollisions();
    void CalculateTableBorders();
    void BallsRotation();
};