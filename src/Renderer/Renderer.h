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
#include "Mesh/Mesh.h"
#include "Sphere.h"

class Renderer
{
public:
    Renderer(int width, int height, const std::string &title);
    ~Renderer();
    double deltaTime;
    bool Init();
    void Display();

private:
    GLFWwindow *window = nullptr;
    int width, height;
    std::string title;
    double currentFrame, lastFrame;
    VAO* vao = nullptr;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
    Shader* shader = nullptr;

    //for table
    Mesh* table_mesh;
    //for sphere
    Sphere* create_Sphere;
    Mesh* sphere_Mesh;
    //for minimap
    Mesh* tableMesh = nullptr;

    void SetupOpenGL();
    void PrintSystemInfo();
};