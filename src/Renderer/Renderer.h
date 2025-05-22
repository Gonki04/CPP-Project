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
#include "Inputs/InputController.h"
#include <tuple>
#include "../Texture/Texture.h"
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
        std::string title;
        double currentFrame, lastFrame;
        VAO *vao = nullptr;
        VBO *vbo = nullptr;
        EBO *ebo = nullptr;
        Shader shader;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<Mesh> poolBalls;
        std::vector<glm::vec3> ballPositions;
        void DrawPoolBalls();
        void GeneratePoolBalls();
        void AnimateBall();

        void SetupOpenGL();
        void PrintSystemInfo();
        void SetCallbacksCam();
    };
}