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
#include "../Minimap/Minimap.h"
#pragma once

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Callback para redimensionamento da janela

namespace Render
{
    class Renderer
    {
    public:
        Renderer(int width, int height, const std::string &title); // Construtor que inicializa a largura, altura e título da janela
        ~Renderer(); // Destrutor que limpa os recursos alocados
        double deltaTime; // Tempo entre frames
        bool Init(); // Inicializa o GLFW, GLEW e cria a janela
        void Display(); // Função principal de renderização que executa o loop de renderização

    private:
        std::map<std::string, Material> materials; // Mapa de materiais carregados do ficheiro MTL
        GLFWwindow *window = nullptr; // Ponteiro para a janela GLFW
        int width, height; // Largura e altura da janela
        Render::Camera camera = Camera(width, height, glm::vec3(-20.0f, 5.0f, 0.0f)); // Instância da câmara com posição inicial
        std::string title; // Título da janela
        double currentFrame, lastFrame; // Variáveis para controlar o tempo entre frames
        VAO *vao = nullptr; // Vertex Array Object para armazenar os atributos dos vértices
        VBO *vbo = nullptr; // Vertex Buffer Object para armazenar os dados dos vértices
        EBO *ebo = nullptr; // Element Buffer Object para armazenar os índices dos vértices
        Shader shader; // Shader para renderização
        std::vector<Vertex> vertices; // Vetor de vértices que contém as posições, normais e coordenadas de textura
        std::vector<unsigned int> indices; // Vetor de índices para o Element Buffer Object
        std::vector<Texture> textures; // Vetor de texturas a serem aplicadas
        
        void SetupOpenGL(); // Configurações iniciais do OpenGL
        void PrintSystemInfo(); // Imprime informações do sistema e do OpenGL
        void SetCallbacksCam(); // Configura os callbacks para a câmara
    };
}