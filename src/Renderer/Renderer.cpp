#include "Renderer.h"

namespace Render
{

    // Callback de erro do GLFW
    static void GLFWErrorCallback(int error, const char *description)
    {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    // Construtor do Renderer
    Renderer::Renderer(int width, int height, const std::string &title)
        : width(width), height(height), title(title)
    {
        // Define o callback de erro do GLFW antes de qualquer outra chamada
        glfwSetErrorCallback(GLFWErrorCallback);

        // Inicializa o tempo dos frames
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // Inicializa o contexto OpenGL, janela e recursos principais
    bool Renderer::Init()
    {
        // Inicializa o GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Configura a versão e o perfil do OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Ativa contexto de debug se necessário

        // Cria a janela principal da aplicação
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Torna o contexto OpenGL da janela atual
        glfwMakeContextCurrent(window);

        // Define o callback para redimensionamento da janela
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Esconde e captura o cursor do mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Inicializa o GLEW 
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
            glfwTerminate();
            return false;
        }

        // Ativa o debug do OpenGL, se disponível
        if (GLEW_ARB_debug_output)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity,
                                         GLsizei length, const GLchar *message, const void *userParam)
                                      { std::cerr << "OpenGL Debug: " << message << std::endl; },
                                      nullptr);
        }

        // Configurações iniciais do OpenGL
        SetupOpenGL();

        // Imprime informações do sistema grafico no terminal
        PrintSystemInfo();

        // Carrega o shader padrão
        shader.LoadShader("resources/default.vert", "resources/default.frag");
        return true;
    }

    // Configurações iniciais do OpenGL
    void Renderer::SetupOpenGL()
    {
        glViewport(0, 0, width, height);         // Define o tamanho da área de renderização
        glEnable(GL_DEPTH_TEST);                 // Ativa o teste de profundidade
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);    // Define a cor de fundo
    }

    // Imprime informações sobre a GPU e o OpenGL
    void Renderer::PrintSystemInfo()
    {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    }

    // Função principal de renderização e loop do programa
    void Renderer::Display()
    {
        shader.Activate(); // Ativa o shader principal

        // Cria o objeto das bolas de bilhar
        Balls *BallsAnimation = new Balls(shader);

        // Carrega a mesa
        Mesh mesh_table(shader, "resources/Assets/table2.obj");
        mesh_table.name = "table";

        // Cria o InputController
        InputController *inputController = nullptr;
        inputController = new InputController(&camera);
        inputController->SetTableMesh(&mesh_table);
        inputController->SetBalls(BallsAnimation);

        // Centraliza a camera no centro da mesa
        glm::vec3 tableCenter = mesh_table.GetCenter();
        camera.Position = glm::vec3(0.0f, 40.0f, 70.0f);
        camera.Orientation = glm::normalize(tableCenter - camera.Position);

        // Registra os callbacks de input na janela
        if (inputController)
        {
            inputController->SetCallbacks(window);
        }
        else
        {
            std::cerr << "Failed to create InputController" << std::endl;
            return;
        }

        // Loop principal
        while (!glfwWindowShouldClose(window))
        {
            // Atualiza o tempo dos frames
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Limpa a tela e o buffer de profundidade
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // matriz global
            glm::mat4 globalRotationMatrix = inputController->GetGlobalRotationMatrix();

            // Define posições e direções iniciais das luzes
            glm::vec3 originalPointLightPos = glm::vec3(0.0f, 5.0f, 0.0f);                                       
            glm::vec3 originalSpotLightPos = glm::vec3(0.0f, 15.0f, 0.0f);                                        
            glm::vec3 originalSpotLightDir = glm::normalize(glm::vec3(0.0f, -10.0f, 0.0f)); 

            // Configura a luz ambiente
            inputController->lights[3]->SetAmbient(glm::vec3(2.0f));

            // Passa a posição da camera para o shader
            shader.SetVec3("viewPos", camera.Position);

            // Atualiza os valores das luzes no shader
            inputController->lights[0]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[1]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[2]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[3]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);

            // Atualiza a matriz de model view da câmera no shader
            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");

            // Renderiza a mesa de bilhar
            mesh_table.Render(glm::vec3(0.0f), glm::vec3(0.0f), globalRotationMatrix);

            // Atualiza o ponteiro das bolas no controlador de entrada
            inputController->SetBalls(BallsAnimation);

            // Atualiza e renderiza as bolas de bilhar
            BallsAnimation->BallsControl(window, deltaTime, globalRotationMatrix);

            // Troca os buffers de renderização
            glfwSwapBuffers(window);

            // Processa eventos de input (teclado, mouse, etc)
            glfwPollEvents();
        }
    }

    Renderer::~Renderer()
    {
        delete vao;
        delete vbo;
        delete ebo;

        if (window)
        {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }
}

// Callback chamado quando a janela é redimensionada
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // definir a janela para novas tamanhos
    glViewport(0, 0, width, height);
}