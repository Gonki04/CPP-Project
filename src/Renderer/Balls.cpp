#include "Balls.h"

namespace Render
{

    static bool onAnimationEvent[16] = {false};
    static double speed[16] = {0.0f};

    void Balls::BallsControl(GLFWwindow *window, double deltaTime)
    {
        DrawPoolBalls();
        AnimateBall(window, deltaTime);
        DetectBallsCollisions();
        CalculateTableBorders();
        BallsRotation();
    }

    void Balls::GeneratePoolBalls(Shader &shader)
    {
        std::string obj_model_filepath = "resources/Assets/";
        std::string obj_model_fileextension = ".obj";
        ballPositions.clear(); // Limpa as posições

        for (int i = 1; i < 16; ++i)
        {
            std::string obj_model_filename = "Ball" + std::to_string(i) + obj_model_fileextension;
            std::string full_path = obj_model_filepath + obj_model_filename;
            Mesh ball(shader, full_path);
            ball.name = "Ball" + std::to_string(i);

            poolBalls.push_back(ball);
            ballPositions.push_back(glm::vec3(0.0f, 4.0f, 20.0f));
            ballOrientations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }

    void Balls::DrawPoolBalls()
    {
        // Parameters for triangle layout
        float ballRadius = 1.0f;                      // Adjust to your model's scale
        float rowSpacing = ballRadius * 2.0f;         // Distance between rows
        float colSpacing = ballRadius * 2.0f * 0.87f; // 0.87 ≈ sqrt(3)/2 for equilateral triangle

        glm::vec3 basePosition = glm::vec3(0.0f, 4.0f, 20.0f); // Center of the triangle base

        int ballIndex = 0;
        for (int row = 0; row < 5; ++row)
        {
            int ballsInRow = row + 1;
            // Center the row horizontally
            float rowZ = basePosition.z + (row * rowSpacing);
            float rowStartX = basePosition.x - (colSpacing * (ballsInRow - 1) / 2.0f);

            for (int col = 0; col < ballsInRow; ++col)
            {
                if (ballIndex >= poolBalls.size())
                    return;

                if (ballPositions[ballIndex] == glm::vec3(0.0f, 4.0f, 20.0f))
                {
                    float x = rowZ;
                    float y = basePosition.y;
                    float z = rowStartX + col * colSpacing;

                    poolBalls[ballIndex].Render(glm::vec3(x, y, z), ballOrientations[ballIndex]);
                    ballPositions[ballIndex] = glm::vec3(x, y, z);
                }
                else
                {
                    poolBalls[ballIndex].Render(ballPositions[ballIndex], ballOrientations[ballIndex]);
                }
                ++ballIndex;
            }
        }
    }

    void Balls::ResetBall(int index, const glm::vec3 &pos)
    {
        if (index >= 0 && index < ballPositions.size())
        {
            ballPositions[index] = pos;
            onAnimationEvent[index] = true;
        }
    }

    void Balls::SetBallSpeed(int index, double newSpeed)
    {
        if (index >= 0 && index < 16)
        {
            speed[index] = newSpeed;
        }
    }

    void Balls::AnimateBall(GLFWwindow *window, double deltaTime)
    {
        onAnimationEvent[0] = true;
        /*
            if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            {
                ballPositions[1] = glm::vec3(-20.0f, 4.0f, 0.0f); // Reset position of ball 1
                onAnimationEvent[1] = true;
            }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                speed[0] = 10.0f;
            }
        */
        // Atualiza a posição das bola se estiver se movendo, conforme a suas velocidades
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (onAnimationEvent[i])
            {
                ballPositions[i].x -= speed[i] * static_cast<float>(deltaTime);
            }
        }
    }

    void Balls::DetectBallsCollisions()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            for (size_t j = i + 1; j < ballPositions.size(); ++j)
            {
                float distance = glm::distance(ballPositions[i], ballPositions[j]);
                if (onAnimationEvent[i])
                {
                    if (distance < 2.0f)
                    {
                        std::cout << "Collision detected between ball " << i << " and ball " << j << std::endl;
                        speed[j] = speed[i] * 0.8f;
                        speed[i] = 0.0f;
                    }
                }
            }
        }
    }

    void Balls::CalculateTableBorders()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (onAnimationEvent[i])
            {
                if (ballPositions[i].x < -36.0f || ballPositions[i].x > 36.0f)
                {
                    std::cout << "Ball " << i << " is out of bounds!" << std::endl;
                    speed[i] = 0.0f;
                    onAnimationEvent[i] = false;
                }
            }
        }
    }

    void Balls::BallsRotation()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            static float rotationAngle = 0.0f;

            if (onAnimationEvent[i])
            {
                if (speed[i] != 0.0f)
                {
                    rotationAngle += speed[i];

                    ballOrientations[i] = glm::vec3(0.0f, 0.0f, rotationAngle);
                }
            }
        }
    }
}