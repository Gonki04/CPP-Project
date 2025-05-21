#pragma once
#include "../headers.h"
#include <GL/glew.h>
#include <Camera/Camera.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader/shaderClass.h"
#include "Mesh/Mesh.h"
#include <tuple>
#pragma once

namespace Render
{
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
        VAO *vao = nullptr;
        VBO *vbo = nullptr;
        EBO *ebo = nullptr;
        Shader shader;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        void SetupOpenGL();
        void PrintSystemInfo();
    };
}