#include "Renderer.h"

namespace Render
{
    Camera camera(800, 600, glm::vec3(-20.0f, 5.0f, 0.0f));
    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    // Error callback for GLFW
    static void GLFWErrorCallback(int error, const char *description)
    {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    Renderer::Renderer(int width, int height, const std::string &title)
        : width(width), height(height), title(title)
    {
        std::cout << "Renderer constructor start" << std::endl;
        // Set error callback first
        glfwSetErrorCallback(GLFWErrorCallback);

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        std::cout << "Renderer constructor end" << std::endl;
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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        shader.LoadShader("resources/default.vert", "resources/default.frag");
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
        Mesh mesh_table("resources/Assets/table.obj");
        Mesh mesh_ball1("resources/Assets/Ball1.obj");

        glfwSetCursorPosCallback(window, mouse_callback);
        while (!glfwWindowShouldClose(window))
        {
            glm::vec3 lightPos = glm::vec3(4.0f, 4.0f, 4.0f);
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.Activate();
            shader.SetVec3("lightPos", lightPos);
            shader.SetVec3("viewPos", camera.Position);
            shader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
            shader.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
            shader.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            shader.SetFloat("material.shininess", 32.0f);

            camera.HandleKeyboardInput(window, deltaTime);
            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");
            std::cout << "It has looped" << std::endl;
            mesh_table.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), shader);
            mesh_ball1.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), shader);

            // table_Mesh.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            // drawMinimap(*table_Mesh, *sphere_Mesh, shader, width, height);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    // load tava aqui

    // install tava aqui

    // redenr tava aqui

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

    void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
        static bool firstMouse = true;
        static double lastX = 0.0;
        static double lastY = 0.0;

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(lastY - ypos); // reversed: y ranges bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= 0.1f; // Adjust sensitivity
        yoffset *= 0.1f; // Adjust sensitivity

        // Update camera orientation based on mouse movement
        camera.ProcessMouseRotation(window, xpos, ypos);
    }

}