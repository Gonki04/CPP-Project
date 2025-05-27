#include "Balls.h"

namespace Render
{

    static bool onAnimationEvent[16] = {false};
    static double speed[16] = {0.0f};

    void Balls::BallsControl(GLFWwindow *window, double deltaTime, glm::mat4 &globalRotationMatrix)
    {
        DrawPoolBalls(globalRotationMatrix);
        AnimateBall(window, deltaTime);
        DetectBallsCollisions();
        CalculateTableBorders();
        BallsRotation();
    }

    void Balls::GeneratePoolBalls(Shader &shader)
    {
        std::string obj_model_filepath = "resources/Assets/";
        std::string obj_model_fileextension = ".obj";

        for (int i = 1; i < 16; ++i)
        {
            std::string obj_model_filename = "Ball" + std::to_string(i) + obj_model_fileextension;
            std::string full_path = obj_model_filepath + obj_model_filename;
            Mesh ball(shader, full_path);
            ball.name = "Ball" + std::to_string(i);

            poolBalls.push_back(ball);
            ballPositions.push_back(glm::vec3(0.0f, 4.0f, 20.0f));   // posição inicial de todas as bolas
            ballOrientations.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // orientação inicial de todas as bolas
        }
    }

    void Balls::DrawPoolBalls(glm::mat4 &globalRotationMatrix)
    {
        // Parameters for triangle layout
        float ballRadius = 1.0f;                      // Adjust to your model's scale
        float rowSpacing = ballRadius * 10.0f;        // distancia entre as linhas
        float colSpacing = ballRadius * 2.0f * 0.87f; // 0.87 ≈ sqrt(3)/2 for equilateral triangle

        glm::vec3 basePosition = glm::vec3(0.0f, 4.0f, -25.0f); // posição do centro do triângulo

        int ballIndex = 0;
        // organizar inicialmente as bolas em um triângulo e depois manter o loop de renderização
        for (int row = 0; row < 5; ++row){
            int ballsInRow = row + 1;
            
            float rowZ = basePosition.z + (row * rowSpacing);
            float rowStartX = basePosition.x - (colSpacing * (ballsInRow - 1) / 2.0f);

            for (int col = 0; col < ballsInRow; ++col)
            {
                if (ballIndex >= poolBalls.size())
                {
                    return; // encerra o loop assim que tods as bolas forem renderizadas
                }
                if (ballPositions[ballIndex] == glm::vec3(0.0f, 4.0f, 20.0f))  // redefine as bolas que estiverem na posição inicial
                {
                    float x = rowZ;
                    float y = basePosition.y;
                    float z = rowStartX + col * colSpacing;

                    poolBalls[ballIndex].Render(glm::vec3(x, y, z), ballOrientations[ballIndex], globalRotationMatrix);
                    ballPositions[ballIndex] = glm::vec3(x, y, z);
                }
                else // loop de renderização das bolas
                {
                    poolBalls[ballIndex].Render(ballPositions[ballIndex], ballOrientations[ballIndex], globalRotationMatrix);
                }
                ++ballIndex;
            }
        }
    }

    void Balls::ResetBall()
    {
        // Reseta a posição e velocidade de todas as bolas
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            ballPositions[i] = glm::vec3(0.0f, 4.0f, 20.0f);    // Posição inicial
            speed[i] = 0.0f;                                    // Velocidade inicial
            onAnimationEvent[i] = false;                        // Reseta o estado de animação
            ballOrientations[i] = glm::vec3(0.0f, 0.0f, 0.0f);  // Orientação inicial
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
        // Atualiza a posição das bola se estiver se movendo, conforme a suas velocidades
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (onAnimationEvent[i])
            {
                ballPositions[i].x += speed[i] * static_cast<float>(deltaTime);
            }
        }
    }

    void Balls::DetectBallsCollisions()
    {
        bool ballsAfected[16] = {false};   // Array to track which balls are affected by collisions
        bool causeCollision[16] = {false}; // Array to track which balls are causing collisions
        float collisionSpeed[16] = {0.0f}; // Array to track the speed of balls involved in collisions

        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            for (size_t j = i + 1; j < ballPositions.size(); ++j)
            {
                float distance = glm::distance(ballPositions[i], ballPositions[j]);
                if (onAnimationEvent[i])

                    if (speed[i] > speed[j])
                    {
                        if (distance < 2.0f)
                        {
                            std::cout << "Collision detected between ball " << i << " and ball " << j << std::endl;
                            causeCollision[i] = true;
                            collisionSpeed[j] = 0.8f * speed[i];
                            ballsAfected[j] = true;
                        }
                    }
            }
        }

        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (causeCollision[i])
            {
                speed[i] = 0.0f;
                onAnimationEvent[i] = false;
            }
            if (ballsAfected[i])
            {
                speed[i] += collisionSpeed[i];
                onAnimationEvent[i] = true;
            }
        }
    }

    void Balls::CalculateTableBorders()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (ballPositions[i].x < -36.0f || ballPositions[i].x > 36.0f)
            {
                std::cout << "Ball " << i << " is out of bounds!" << std::endl;
                speed[i] = 0.0f;
                onAnimationEvent[i] = false;
            }
        }
    }

    void Balls::BallsRotation()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            static float rotationAngle[16] = {0.0f};

            if (onAnimationEvent[i])
            {
                if (speed[i] != 0.0f)
                {
                    rotationAngle[i] -= (speed[i] * 0.5f);

                    ballOrientations[i] = glm::vec3(0.0f, 0.0f, rotationAngle[i]);
                }
            }
        }
    }
}