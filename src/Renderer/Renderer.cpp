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
        GeneratePoolBalls();
        Mesh mesh_table(shader, "resources/Assets/table2.obj");
        mesh_table.name = "table";

        InputController *inputController = nullptr;
        glm::vec3 Tpose = mesh_table.GetCenter();
        camera.Position = Tpose + glm::vec3(0.0f, 0.0f, 70.0f);
        camera.Orientation = glm::normalize(Tpose - camera.Position);

        inputController = new InputController(&camera, &mesh_table);

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
            shader.SetVec3("lightPos", lightPos);
            shader.SetVec3("viewPos", camera.Position);

            static bool ambientEnabled = true;
            static bool directionalEnabled = true;
            static bool pointEnabled = true;
            static bool spotEnabled = true;
            std::cout << "ambientEnabled: " << ambientEnabled << std::endl;
            std::cout << "directionalEnabled: " << directionalEnabled << std::endl;
            std::cout << "pointEnabled: " << pointEnabled << std::endl;
            std::cout << "spotEnabled: " << spotEnabled << std::endl;

            shader.SetInt("ambientLight.enabled", ambientEnabled ? 1 : 0);
            shader.SetVec3("ambientLight.color", glm::vec3(0.1f, 0.1f, 0.1f)); // Soft gray ambient
            shader.SetInt("directionalLight.enabled", directionalEnabled ? 1 : 0);
            shader.SetVec3("directionalLight.direction", glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)));
            shader.SetVec3("directionalLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            shader.SetVec3("directionalLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.SetVec3("directionalLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetInt("pointLight.enabled", pointEnabled ? 1 : 0);
            shader.SetVec3("pointLight.position", glm::vec3(0.0f, 10.0f, 0.0f));
            shader.SetVec3("pointLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            shader.SetVec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.SetVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetFloat("pointLight.kc_atenuation", 1.0f);   // constant
            shader.SetFloat("pointLight.kl_atenuation", 0.09f);  // linear
            shader.SetFloat("pointLight.kq_atenuation", 0.032f); // quadratic
            shader.SetInt("spotLight.enabled", spotEnabled ? 1 : 0);
            glm::vec3 ballsBase = glm::vec3(0.0f, 4.0f, 20.0f);
            float triangleHeight = 4 * (1.0f * 2.0f); // 4 rows after the base, spacing = ballRadius*2
            glm::vec3 ballsCenter = ballsBase + glm::vec3(0.0f, 0.0f, triangleHeight / 2.0f);
            shader.SetVec3("spotLight.position", camera.Position);
            glm::vec3 spotDirection = glm::normalize(ballsCenter - camera.Position);
            shader.SetVec3("spotLight.direction", spotDirection);
            shader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
            shader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetFloat("spotLight.kc_atenuation", 1.0f);
            shader.SetFloat("spotLight.kl_atenuation", 0.09f);
            shader.SetFloat("spotLight.kq_atenuation", 0.032f);
            shader.SetFloat("spotLight.s_exponent", 32.0f);
            shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));      // inner cone
            shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f))); // outer cone

            static int prev1 = GLFW_RELEASE, prev2 = GLFW_RELEASE, prev3 = GLFW_RELEASE, prev4 = GLFW_RELEASE;

            int state1 = glfwGetKey(window, GLFW_KEY_1);
            int state2 = glfwGetKey(window, GLFW_KEY_2);
            int state3 = glfwGetKey(window, GLFW_KEY_3);
            int state4 = glfwGetKey(window, GLFW_KEY_4);

            if (state1 == GLFW_PRESS && prev1 == GLFW_RELEASE)
                ambientEnabled = !ambientEnabled;
            if (state2 == GLFW_PRESS && prev2 == GLFW_RELEASE)
                directionalEnabled = !directionalEnabled;
            if (state3 == GLFW_PRESS && prev3 == GLFW_RELEASE)
                pointEnabled = !pointEnabled;
            if (state4 == GLFW_PRESS && prev4 == GLFW_RELEASE)
                spotEnabled = !spotEnabled;

            prev1 = state1;
            prev2 = state2;
            prev3 = state3;
            prev4 = state4;

            shader.SetInt("ambientLight.enabled", ambientEnabled ? 1 : 0);
            shader.SetInt("directionalLight.enabled", directionalEnabled ? 1 : 0);
            shader.SetInt("pointLight.enabled", pointEnabled ? 1 : 0);
            shader.SetInt("spotLight.enabled", spotEnabled ? 1 : 0);

            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");

            mesh_table.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            DrawPoolBalls();
            // mesh_table.Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
            // mesh_ball1.Render(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.25f, 0.25f, 0.25f));

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
            ball.name = "Ball" + std::to_string(i);

            poolBalls.push_back(ball);
        }
    }

    void Renderer::DrawPoolBalls()
    {
        // Parameters for triangle layout
        float ballRadius = 1.0f;                      // Adjust to your model's scale
        float rowSpacing = ballRadius * 2.0f;         // Distance between rows
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}