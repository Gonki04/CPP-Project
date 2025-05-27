#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Shader/shaderClass.h"

namespace Render{

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);

    int width;
    int height;

    float fov_ = 45.0f;         // Initial field of view

    Camera(int width, int height, glm::vec3 position);

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
    void HandleScroll(double yoffset);
private:
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
};


}

