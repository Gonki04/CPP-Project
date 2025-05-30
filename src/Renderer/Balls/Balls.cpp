#include "Balls.h"

namespace Render
{

    static bool onAnimationEvent[16] = {false};
    static double speed[16] = {0.0f};

    void Balls::BallsControl(GLFWwindow *window, double deltaTime,glm::mat4 &globalRotationMatrix)
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
            ballPositions.push_back(glm::vec3(0.0f, 4.0f, 20.0f));   
            ballOrientations.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); 
        }
    }

    
    void Balls::DrawPoolBalls(glm::mat4 &globalRotationMatrix)
    {
        
        float ballRadius = 1.0f;                      
        float rowSpacing = ballRadius * 10.0f;        
        float colSpacing = ballRadius * 2.0f * 0.87f; 

        glm::vec3 basePosition = glm::vec3(0.0f, 4.0f, -25.0f); 

        int ballIndex = 0;
        for (int row = 0; row < 5; ++row)
        {
            int ballsInRow = row + 1;

            float rowZ = basePosition.z + (row * rowSpacing);                           
            float rowStartX = basePosition.x - (colSpacing * (ballsInRow - 1) / 2.0f);  

            for (int col = 0; col < ballsInRow; ++col)
            {
                if (ballIndex >= poolBalls.size())
                {
                    return; 
                }
                if (ballPositions[ballIndex] == glm::vec3(0.0f, 4.0f, 20.0f)) 
                {
                    float x = rowZ;                         
                    float y = basePosition.y;               
                    float z = rowStartX + col * colSpacing; 

                     poolBalls[ballIndex].Render(glm::vec3(x, y, z), ballOrientations[ballIndex], globalRotationMatrix);
                    ballPositions[ballIndex] = glm::vec3(x, y, z);  
                }
                else 
                {
                    poolBalls[ballIndex].Render(ballPositions[ballIndex], ballOrientations[ballIndex], globalRotationMatrix);
                }
                ++ballIndex;
            }
        }
    }

    
    void Balls::ResetBall()
    {
        for (size_t i = 0; i < ballPositions.size(); ++i)
        {
            ballPositions[i] = glm::vec3(0.0f, 4.0f, 20.0f);   
            speed[i] = 0.0f;                                   
            onAnimationEvent[i] = false;                       
            ballOrientations[i] = glm::vec3(0.0f, 0.0f, 0.0f); 
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
        bool ballsAfected[16] = {false};   
        bool causeCollision[16] = {false}; 
        float collisionSpeed[16] = {0.0f}; 

        
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