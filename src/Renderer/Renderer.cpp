#include "Renderer.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader/shaderClass.h"
#include "Minimap/Minimap.h"

std::vector<Vertex> vertices = {
    // Frente (Front)
    {{-2.5f, -0.1f, 1.25f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.3f, 0.0f}, {0.0f, 0.0f}},
    {{2.5f, -0.1f, 1.25f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.3f, 0.0f}, {1.0f, 0.0f}},
    {{2.5f, 0.1f, 1.25f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.3f, 0.0f}, {1.0f, 1.0f}},
    {{-2.5f, 0.1f, 1.25f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.3f, 0.0f}, {0.0f, 1.0f}},

    // Trás (Back)
    {{-2.5f, -0.1f, -1.25f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f}},
    {{-2.5f, 0.1f, -1.25f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.5f, 0.0f}, {1.0f, 1.0f}},
    {{2.5f, 0.1f, -1.25f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 1.0f}},
    {{2.5f, -0.1f, -1.25f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 0.0f}},

    // Direita (Right)
    {{2.5f, -0.1f, 1.25f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.7f, 0.0f}, {0.0f, 0.0f}},
    {{2.5f, -0.1f, -1.25f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.7f, 0.0f}, {1.0f, 0.0f}},
    {{2.5f, 0.1f, -1.25f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.7f, 0.0f}, {1.0f, 1.0f}},
    {{2.5f, 0.1f, 1.25f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.7f, 0.0f}, {0.0f, 1.0f}},

    // Esquerda (Left)
    {{-2.5f, -0.1f, 1.25f}, {-1.0f, 0.0f, 0.0f}, {0.3f, 0.8f, 0.3f}, {1.0f, 0.0f}},
    {{-2.5f, 0.1f, 1.25f}, {-1.0f, 0.0f, 0.0f}, {0.3f, 0.8f, 0.3f}, {1.0f, 1.0f}},
    {{-2.5f, 0.1f, -1.25f}, {-1.0f, 0.0f, 0.0f}, {0.3f, 0.8f, 0.3f}, {0.0f, 1.0f}},
    {{-2.5f, -0.1f, -1.25f}, {-1.0f, 0.0f, 0.0f}, {0.3f, 0.8f, 0.3f}, {0.0f, 0.0f}},

    // Cima (Top)
    {{-2.5f, 0.1f, 1.25f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f, 0.5f}, {0.0f, 1.0f}},
    {{2.5f, 0.1f, 1.25f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}},
    {{2.5f, 0.1f, -1.25f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f, 0.5f}, {1.0f, 0.0f}},
    {{-2.5f, 0.1f, -1.25f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f, 0.5f}, {0.0f, 0.0f}},

    // Baixo (Bottom)
    {{-2.5f, -0.1f, 1.25f}, {0.0f, -1.0f, 0.0f}, {0.6f, 1.0f, 0.6f}, {0.0f, 0.0f}},
    {{-2.5f, -0.1f, -1.25f}, {0.0f, -1.0f, 0.0f}, {0.6f, 1.0f, 0.6f}, {0.0f, 1.0f}},
    {{2.5f, -0.1f, -1.25f}, {0.0f, -1.0f, 0.0f}, {0.6f, 1.0f, 0.6f}, {1.0f, 1.0f}},
    {{2.5f, -0.1f, 1.25f}, {0.0f, -1.0f, 0.0f}, {0.6f, 1.0f, 0.6f}, {1.0f, 0.0f}}};

std::vector<unsigned int> indices = {
    // Frente
    0, 1, 2,
    2, 3, 0,

    // Trás
    4, 5, 6,
    6, 7, 4,

    // Direita
    8, 9, 10,
    10, 11, 8,

    // Esquerda
    12, 13, 14,
    14, 15, 12,

    // Cima
    16, 17, 18,
    18, 19, 16,

    // Baixo
    20, 21, 22,
    22, 23, 20};

// Error callback for GLFW
static void GLFWErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

Renderer::Renderer(int width, int height, const std::string &title)
    : width(width), height(height), title(title)
{
    // Set error callback first
    glfwSetErrorCallback(GLFWErrorCallback);
}

bool Renderer::Init()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // For debug output

    // Create window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
        glfwTerminate();
        return false;
    }

    // Enable debug output
    if (GLEW_ARB_debug_output)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity,
                                     GLsizei length, const GLchar *message, const void *userParam)
                                  { std::cerr << "OpenGL Debug: " << message << std::endl; },
                                  nullptr);
    }

    SetupOpenGL();
    PrintSystemInfo();

    vao = new VAO();
    vao->Bind();

    ebo = new EBO(indices); // EBO deve ser criado/bindado com VAO ativo
    vbo = new VBO(vertices);

    vao->LinkVBO(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, position));
    vao->LinkVBO(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    vao->LinkVBO(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, color));
    vao->LinkVBO(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, texUV));

    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();

    shader = new Shader("resources/default.vert", "resources/default.frag");

    // Table mesh for minimap
    tableMesh = new Mesh(vertices, indices);

    return true;
}

void Renderer::SetupOpenGL()
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

void Renderer::PrintSystemInfo()
{
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
}

void Renderer::Display()
{
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Activate();
        vao->Bind();

        static float angle = 0.0f;
        angle += 0.01f;
        float radius = 6.0f;
        float camY = 2.5f;
        float camX = sin(angle) * radius;
        float camZ = cos(angle) * radius;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

        GLuint modelLoc = glGetUniformLocation(shader->ID, "u_Model");
        GLuint viewLoc = glGetUniformLocation(shader->ID, "u_View");
        GLuint projLoc = glGetUniformLocation(shader->ID, "u_Projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        drawMinimap(*tableMesh, shader, width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Renderer::~Renderer()
{
    delete vao;
    delete vbo;
    delete ebo;
    delete shader;
    delete tableMesh;
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}