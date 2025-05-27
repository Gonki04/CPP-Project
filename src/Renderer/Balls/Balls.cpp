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

    // carrega os modelos das bolas a partir do files usando a funçao 
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

    // organizar inicialmente as bolas em um triângulo e depois manter o loop de renderização
    void Balls::DrawPoolBalls()
    {
        // Parameters for triangle layout
        float ballRadius = 1.0f;                      // escala das bolas
        float rowSpacing = ballRadius * 10.0f;        // distancia entre as linhas
        float colSpacing = ballRadius * 2.0f * 0.87f; // distancia entre as bolas

        glm::vec3 basePosition = glm::vec3(0.0f, 4.0f, -25.0f); // posição do centro do triângulo

        int ballIndex = 0;
        for (int row = 0; row < 5; ++row)
        {
            int ballsInRow = row + 1;

            float rowZ = basePosition.z + (row * rowSpacing);                           //
            float rowStartX = basePosition.x - (colSpacing * (ballsInRow - 1) / 2.0f);  //

            for (int col = 0; col < ballsInRow; ++col)
            {
                if (ballIndex >= poolBalls.size())
                {
                    return; // encerra o loop assim que tods as bolas forem renderizadas
                }
                if (ballPositions[ballIndex] == glm::vec3(0.0f, 4.0f, 20.0f)) // redefine as bolas que estiverem na posição inicial
                {
                    float x = rowZ;                         //
                    float y = basePosition.y;               //
                    float z = rowStartX + col * colSpacing; //

                    poolBalls[ballIndex].Render(glm::vec3(x, y, z), ballOrientations[ballIndex]);
                    ballPositions[ballIndex] = glm::vec3(x, y, z);  // guarda a posição das bolas numa variável
                }
                else // loop de renderização das bolas
                {
                    poolBalls[ballIndex].Render(ballPositions[ballIndex], ballOrientations[ballIndex]);
                }
                ++ballIndex;
            }
        }
    }

    // Reseta a posição e velocidade de todas as bolas
    void Balls::ResetBall()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            ballPositions[i] = glm::vec3(0.0f, 4.0f, 20.0f);   // Posição inicial
            speed[i] = 0.0f;                                   // Velocidade inicial
            onAnimationEvent[i] = false;                       // Reseta o estado de animação
            ballOrientations[i] = glm::vec3(0.0f, 0.0f, 0.0f); // Orientação inicial
        }
    }

    // Define uma velocidade para uma bola específica (usado no InputController)
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
        // Atualiza a posição das bola se estiver se movendo, d acordo a suas velocidades
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
        bool ballsAfected[16] = {false};   // defenir que bolas foram afetadas por colisões
        bool causeCollision[16] = {false}; // defenir que bolas causaram colisões
        float collisionSpeed[16] = {0.0f}; // velocidade que as bolas afetadas vão receber das bolas que causaram colisões

        // Verifica colisões entre as bolas
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            for (size_t j = i + 1; j < ballPositions.size(); ++j)
            {
                float distance = glm::distance(ballPositions[i], ballPositions[j]);
                if (onAnimationEvent[i])

                    if (speed[i] > speed[j])
                    {
                        // define qual bola causou a colisao e qual bola foi afetada
                        if (distance < 2.0f)
                        {
                            std::cout << "Collision detected between ball " << i << " and ball " << j << std::endl;
                            causeCollision[i] = true;
                            collisionSpeed[j] = 0.8f * speed[i]; // A bola afetada recebe 80% da velocidade da bola que causou a colisão
                            ballsAfected[j] = true;
                        }
                    }
            }
        }

        // baseado nas colisões detectadas, atualiza a velocidade das bolas
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            if (causeCollision[i]) // a bola que causou a colisao para
            {
                speed[i] = 0.0f;
                onAnimationEvent[i] = false;
            }
            if (ballsAfected[i]) // a bola que foi afetada recebe a velocidade da bola que causou a colisao
            {
                speed[i] += collisionSpeed[i];
                onAnimationEvent[i] = true;
            }
        }
    }

    // Verifica se as bolas estão fora dos limites da mesa (no eixo X)
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

    // Rotaciona as bolas
    void Balls::BallsRotation()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            static float rotationAngle[16] = {0.0f}; // guarda o angulo de rotação de cada bola

            if (onAnimationEvent[i])
            {
                if (speed[i] != 0.0f)
                {
                    rotationAngle[i] -= (speed[i] * 0.5f); // enquanto a bola se move, atualiza o angulo de rotação

                    ballOrientations[i] = glm::vec3(0.0f, 0.0f, rotationAngle[i]);
                }
            }
        }
    }
}