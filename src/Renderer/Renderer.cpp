#include "Renderer.h"

namespace Render
{

    
    static void GLFWErrorCallback(int error, const char *description)
    {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    
    Renderer::Renderer(int width, int height, const std::string &title)
        : width(width), height(height), title(title)
    {
        
        glfwSetErrorCallback(GLFWErrorCallback);

        
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    
    bool Renderer::Init()
    {
        
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        

        
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

        
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
            glfwTerminate();
            return false;
        }

        
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
        camera.Position = glm::vec3(0.0f, 40.0f, 70.0f);
        camera.Orientation = glm::normalize(tableCenter - camera.Position);

        
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
            
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
            glm::mat4 globalRotationMatrix = inputController->GetGlobalRotationMatrix();

            
            glm::vec3 originalPointLightPos = glm::vec3(0.0f, 5.0f, 0.0f);                                       
            glm::vec3 originalSpotLightPos = glm::vec3(0.0f, 15.0f, 0.0f);                                        
            glm::vec3 originalSpotLightDir = glm::normalize(glm::vec3(0.0f, -10.0f, 0.0f)); 

            
            inputController->lights[3]->SetAmbient(glm::vec3(2.0f));

            
            shader.SetVec3("viewPos", camera.Position);

            
            inputController->lights[0]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[1]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[2]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);
            inputController->lights[3]->SetShaderLightValue(shader.ID, originalPointLightPos, originalSpotLightPos, originalSpotLightDir);

            
            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");

            
            mesh_table.Render(glm::vec3(0.0f), glm::vec3(0.0f), globalRotationMatrix);

            
            inputController->SetBalls(BallsAnimation);

            
            BallsAnimation->BallsControl(window, deltaTime, globalRotationMatrix);

            
            drawMinimap(mesh_table, *BallsAnimation, &shader, width, height);

            
            drawMinimap(mesh_table, *BallsAnimation, &shader, width, height);

            
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
    
    glViewport(0, 0, width, height);
}