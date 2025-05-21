#include "Renderer.h"

namespace Render
{
    Camera camera(800, 600, glm::vec3(-20.0f, 5.0f, 0.0f));
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

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
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
        //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // For debug output


        // Create window
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

    void Renderer::SetCallbacks()
    {
        glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
                              { camera.HandleScroll(yoffset); });
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Renderer::Display()
    {
        shader.Activate();
        Mesh mesh_table(shader, "resources/Assets/table2.obj");
        GeneratePoolBalls();

        InputController *inputController = nullptr;
        glm::vec3 Tpose = mesh_table.GetCenter();
        camera.Position = Tpose + glm::vec3(0.0f, 0.0f, 70.0f);
        camera.Orientation = glm::normalize(Tpose - camera.Position);

        inputController = new InputController(&camera, &mesh_table);

        SetCallbacks();

        if (inputController)
        {
            glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos)
                                     {
                InputController* inputController = static_cast<InputController*>(glfwGetWindowUserPointer(window));
                if (inputController)
                {
                    inputController->MouseCallback(window, xpos, ypos);
                } });
            glfwSetWindowUserPointer(window, inputController);
        }

        while (!glfwWindowShouldClose(window))
        {
            glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            shader.SetVec3("lightPos", lightPos);
            shader.SetVec3("viewPos", camera.Position);


            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");

            mesh_table.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            DrawPoolBalls();
            //mesh_table.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            //mesh_ball1.Render(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

            // table_Mesh.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            // drawMinimap(*table_Mesh, *sphere_Mesh, shader, width, height);

            glfwSwapBuffers(window);
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


    void Renderer::GeneratePoolBalls()
    {
        std::string obj_model_filepath = "resources/Assets/";
        std::string obj_model_fileextension = ".obj";

        for (int i = 1; i < 16; ++i)
        {
            std::string obj_model_filename = "Ball" + std::to_string(i) + obj_model_fileextension;
            std::string full_path = obj_model_filepath + obj_model_filename;
            Mesh ball(shader, full_path);

            poolBalls.push_back(ball);
        }

    }

void Renderer::DrawPoolBalls()
{
    // Parameters for triangle layout
    float ballRadius = 1.0f; // Adjust to your model's scale
    float rowSpacing = ballRadius * 2.0f; // Distance between rows
    float colSpacing = ballRadius * 2.0f * 0.87f; // 0.87 ≈ sqrt(3)/2 for equilateral triangle

    glm::vec3 basePosition = glm::vec3(0.0f, 4.0f, 20.0f); // Center of the triangle base

    int ballIndex = 0;
    for (int row = 0; row < 5; ++row)
    {
        int ballsInRow = row + 1;
        // Center the row horizontally
        float rowZ = basePosition.z + (row * rowSpacing);
        float rowStartX = basePosition.x - (colSpacing * (ballsInRow - 1) / 2.0f);

        for (int col = 0; col < ballsInRow; ++col)
        {
            if (ballIndex >= poolBalls.size())
                return;

            float x = rowZ;
            float y = basePosition.y;
            float z = rowStartX + col * colSpacing;

            poolBalls[ballIndex].Render(glm::vec3(x, y, z), glm::vec3(0.0f));
            ++ballIndex;
        }
    }
}


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}