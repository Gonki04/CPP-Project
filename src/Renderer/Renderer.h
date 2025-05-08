#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader/shaderClass.h"

class Renderer
{
public:
    Renderer(int width, int height, const std::string &title);
    ~Renderer();

    bool Init();
    void Display();

private:
    GLFWwindow *window = nullptr;
    int width, height;
    std::string title;

    VAO* vao = nullptr;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
    Shader* shader = nullptr;

    void SetupOpenGL();
    void PrintSystemInfo();
};