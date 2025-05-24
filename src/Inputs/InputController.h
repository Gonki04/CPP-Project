#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera/Camera.h"
#include "../Renderer/Mesh/Mesh.h"

class InputController {
public:
    InputController(Camera* camera);
    ~InputController();

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void CursorCallback(GLFWwindow* window, double xpos, double ypos);
    void SetCallbacks(GLFWwindow* window);
    void SetTableMesh(Mesh* table_Mesh);

    // Luzes
    bool lightKeys[4] = {false, false, false, false};
    bool spacePressed = false;
    float modelYaw = 0.0f;
    float modelPitch = 0.0f;

private:
    Camera* camera;
    Mesh* table_Mesh = nullptr; // Mesh da mesa
    glm::vec3* target = nullptr; // Centro da mesa

    
    float lastX, lastY;
    bool firstMouse;
};