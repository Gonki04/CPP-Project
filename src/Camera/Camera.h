#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Shader/shaderClass.h"

class Camera
{
public:
    glm::vec3 Position;


    int width;
    int height;

    float zoom_ = 45.0f;        // Initial zoom level
    float fov_ = 45.0f;         // Initial field of view
    float sensitivity = 100.0f; // Mouse sensitivity
    float speed = 0.1f;         // Movement speed

    Camera(int width, int height, glm::vec3 position);

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
    void HandleScroll(double yoffset);
    void HandleKeyboardInput(GLFWwindow *window, float deltaSpeed);
    void ProcessMouseRotation(GLFWwindow* window, double xpos, double ypos);
private:
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif //  CAMERA_H
#define CAMERA_H
