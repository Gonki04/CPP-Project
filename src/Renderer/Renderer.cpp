#include "Renderer.h"

namespace Render
{

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
        // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // For debug output

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

    void Renderer::Display()
    {
        shader.Activate();
        Balls *BallsAnimation = new Balls(shader);

        Mesh mesh_table(shader, "resources/Assets/table2.obj");
        mesh_table.name = "table";

        InputController *inputController = nullptr;

        inputController = new InputController(&camera);
        inputController->SetTableMesh(&mesh_table);
        inputController->SetBalls(BallsAnimation);

        glm::vec3 tableCenter = mesh_table.GetCenter();
        camera.Position = glm::vec3(0.0f, 70.0f, 70.0f);
        camera.Orientation = glm::normalize(tableCenter - camera.Position); // Looking down the Z-axis

        if (inputController)
        {
            inputController->SetCallbacks(window);
        }
        else
        {
            std::cerr << "Failed to create InputController" << std::endl;
            return;
        }

        while (!glfwWindowShouldClose(window))
        {
            glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 globalRotationMatrix = inputController->GetGlobalRotationMatrix();

            glm::vec3 originalPointLightPos = glm::vec3(0.0f, 5.0f, 0.0f);                                       // por exemplo
            glm::vec3 originalSpotLightPos = glm::vec3(3.0f, 2.0f, 3.0f);                                        // por exemplo
            glm::vec3 originalSpotLightDir = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - originalSpotLightPos); // por exemplo

            // aplica a rotacao global so as posicoes e direcoes das luzes
            glm::vec3 transformedPointLightPos = glm::vec3(globalRotationMatrix * glm::vec4(originalPointLightPos, 1.0f));
            glm::vec3 transformedSpotLightPos = glm::vec3(globalRotationMatrix * glm::vec4(originalSpotLightPos, 1.0f));
            glm::vec3 transformedSpotLightDir = glm::normalize(glm::vec3(globalRotationMatrix * glm::vec4(originalSpotLightDir, 0.0f))); // w=0 para direcoes

            // agora, envia estas 'transformed...' para o teu shader

            shader.SetVec3("viewPos", camera.Position);

            shader.SetInt("ambientLight.enabled", inputController->ambientEnabled ? 1 : 0);
            shader.SetVec3("ambientLight.color", glm::vec3(0.1f, 0.1f, 0.1f)); // Soft gray ambient
            shader.SetInt("directionalLight.enabled", inputController->directionalEnabled ? 1 : 0);
            shader.SetVec3("directionalLight.direction", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
            shader.SetVec3("directionalLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            shader.SetVec3("directionalLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.SetVec3("directionalLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetInt("pointLight.enabled", inputController->pointEnabled ? 1 : 0);
            shader.SetVec3("pointLight.position", transformedPointLightPos);

            shader.SetVec3("pointLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetVec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.SetVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetFloat("pointLight.kc_atenuation", 1.0f);   // constant
            shader.SetFloat("pointLight.kl_atenuation", 0.09f);  // linear
            shader.SetFloat("pointLight.kq_atenuation", 0.032f); // quadratic
            shader.SetInt("spotLight.enabled", inputController->spotEnabled ? 1 : 0);
            shader.SetVec3("spotLight.position", transformedSpotLightPos);
            shader.SetVec3("spotLight.direction", transformedSpotLightDir);
            shader.SetVec3("spotLight.ambient", glm::vec3(20.5f, 20.5f, 20.5f));
            shader.SetVec3("spotLight.diffuse", glm::vec3(20.0f, 20.0f, 20.0f));
            shader.SetVec3("spotLight.specular", glm::vec3(20.0f, 20.0f, 20.0f));
            shader.SetFloat("spotLight.kc_atenuation", 0.01f);
            shader.SetFloat("spotLight.kl_atenuation", 0.05f);
            shader.SetFloat("spotLight.kq_atenuation", 0.01f);
            shader.SetFloat("spotLight.s_exponent", 32.0f);
            shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(0.5f)));       // inner cone
            shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(30.5f))); // outer cone

            shader.SetInt("ambientLight.enabled", inputController->ambientEnabled ? 1 : 0);
            shader.SetInt("directionalLight.enabled", inputController->directionalEnabled ? 1 : 0);
            shader.SetInt("pointLight.enabled", inputController->pointEnabled ? 1 : 0);
            shader.SetInt("spotLight.enabled", inputController->spotEnabled ? 1 : 0);

            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");

            mesh_table.Render(glm::vec3(0.0f), glm::vec3(0.0f), globalRotationMatrix);

            inputController->SetBalls(BallsAnimation);
            BallsAnimation->BallsControl(window, deltaTime, globalRotationMatrix);

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
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
