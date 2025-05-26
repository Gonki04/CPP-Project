#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera/Camera.h"
#include "../Renderer/Mesh/Mesh.h"
#include "../Renderer/Balls.h"

class Balls;

class InputController
{

public:
    InputController(Camera *camera);
    ~InputController();

    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void CursorCallback(GLFWwindow *window, double xpos, double ypos);
    void SetCallbacks(GLFWwindow *window);
    void SetTableMesh(Mesh *table_Mesh);

    void SetBalls(Balls *ballsPtr) { balls = ballsPtr; }

    // Luzes
    float modelYaw = 0.0f;
    float modelPitch = 0.0f;
    bool ambientEnabled = true;
    bool directionalEnabled = true;
    bool pointEnabled = true;
    bool spotEnabled = true;

private:
    Camera *camera;
    Mesh *table_Mesh = nullptr;  // Mesh da mesa
    glm::vec3 *target = nullptr; // Centro da mesa
    Balls *balls = nullptr;

    int prev1 = GLFW_RELEASE, prev2 = GLFW_RELEASE, prev3 = GLFW_RELEASE, prev4 = GLFW_RELEASE;

    float lastX, lastY;
    bool firstMouse;
};