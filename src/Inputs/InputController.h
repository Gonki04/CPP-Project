#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera/Camera.h"
#include "../Renderer/Mesh/Mesh.h"

class InputController {
public:
    InputController(Camera* camera, Mesh* table_Mesh);
    ~InputController();

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void CursorCallback(GLFWwindow* window, double xpos, double ypos);
    void SetCallbacks(GLFWwindow* window);

    // Luzes
    bool lightKeys[4] = {false, false, false, false};
    bool spacePressed = false;

private:
    Camera* camera;
    Mesh* table_Mesh;
    glm::vec3* target; // Centro da mesa

    float yaw;
    float pitch;
    float lastX, lastY;
    bool firstMouse;
    float distanceToTarget; // Centro da mesa
};