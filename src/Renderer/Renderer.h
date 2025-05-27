#pragma once
#include "../headers.h"
#include <Camera/Camera.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader/shaderClass.h"
#include "Mesh/Mesh.h"
#include "../Inputs/InputController.h"
#include <tuple>
#include "../Texture/Texture.h"
#include "Balls/Balls.h"
#pragma once

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
        std::map<std::string, Material> materials;
        GLFWwindow *window = nullptr;
        int width, height;
        Render::Camera camera = Camera(width, height, glm::vec3(-20.0f, 5.0f, 0.0f));
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
        void SetCallbacksCam();
    };
}