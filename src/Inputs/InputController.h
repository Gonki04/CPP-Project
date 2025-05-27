#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Camera/Camera.h"
#include "../Renderer/Mesh/Mesh.h"
#include "../Renderer/Balls/Balls.h"
#include "../Lights/Lights.h"

namespace Render
{
    class Balls; // Forward declaration to avoid circular dependency
    class InputController
    {
public:
        Light* lights[4];


        
    public:
        InputController(Camera *camera);
        ~InputController();

        void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
        void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void CursorCallback(GLFWwindow *window, double xpos, double ypos);
        void SetCallbacks(GLFWwindow *window);
        void SetTableMesh(Mesh *table_Mesh);
        void SetBalls(Balls *balls_ptr); // Added to set the Balls pointer


        // Luzes
        float modelYaw;
        float modelPitch;


        bool ambientEnabled = true;
        bool directionalEnabled = true;
        bool pointEnabled = true;
        bool spotEnabled = true;


        glm::mat4 GetGlobalRotationMatrix() const;

    private:
        Camera *camera;
        Mesh *table_Mesh = nullptr;  // Mesh da mesa
        glm::vec3 *target = nullptr; // Centro da mesa
        Balls *balls = nullptr;

        int prev1 = GLFW_RELEASE, prev2 = GLFW_RELEASE, prev3 = GLFW_RELEASE, prev4 = GLFW_RELEASE;

        float lastX, lastY;
        bool firstMouse;
    };
}
