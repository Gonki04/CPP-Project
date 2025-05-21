#include "Renderer.h"
#include "../Texture/Texture.h"

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
        shader.Activate();
        Load("resources/Assets/Ball1.obj");
        Install();
        Load("resources/Assets/table.obj");
        Install();
        glfwSetCursorPosCallback(window, mouse_callback);
        while (!glfwWindowShouldClose(window))
        {
            glm::vec3 lightPos = glm::vec3(4.0f, 4.0f, 4.0f);
            currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            shader.SetVec3("lightPos", lightPos);
            shader.SetVec3("viewPos", camera.Position);
            shader.SetMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f)));
            
            camera.HandleKeyboardInput(window, deltaTime);
            camera.Matrix(camera.fov_, 0.1f, 1000.0f, shader, "u_ViewProjection");
            Render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

            // drawMinimap(*table_Mesh, *sphere_Mesh, shader, width, height);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
         // Wait for user input to start rendering

    }

    void Renderer::Load(std::string obj_model_filepath) // Load .obj file
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::map<std::tuple<int, int, int>, unsigned int> uniqueVertexMap;
        std::fstream file(obj_model_filepath, std::ios::in);
        std::map<std::string, Material> materials;
        std::string currentMaterialName;
        std::string mtlFilePath;
        if (!file.is_open())
        {
            std::cerr << "Could not open the file!" << std::endl;
            return;
        }
        std::string line;
        while (getline(file, line))
        {
            // Skip empty lines or comments
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
            }
            else if (prefix == "vt")
            {
                glm::vec2 tex;
                iss >> tex.x >> tex.y;
                texcoords.push_back(tex);
            }
            else if (prefix == "vn")
            {
                glm::vec3 norm;
                iss >> norm.x >> norm.y >> norm.z;
                normals.push_back(norm);
            }
            else if (prefix == "f")
            {
                std::string vertexStr;
                for (int i = 0; i < 3; ++i)
                {
                    iss >> vertexStr;
                    size_t pos1 = vertexStr.find('/');
                    size_t pos2 = vertexStr.find('/', pos1 + 1);

                    int vIdx = std::stoi(vertexStr.substr(0, pos1)) - 1;
                    int vtIdx = std::stoi(vertexStr.substr(pos1 + 1, pos2 - pos1 - 1)) - 1;
                    int vnIdx = std::stoi(vertexStr.substr(pos2 + 1)) - 1;

                    // Check ranges before accessing
                    if (vIdx < 0 || vIdx >= positions.size() ||
                        vtIdx < 0 || vtIdx >= texcoords.size() ||
                        vnIdx < 0 || vnIdx >= normals.size())
                    {
                        std::cerr << "OBJ index out of range: v=" << vIdx << " vt=" << vtIdx << " vn=" << vnIdx << std::endl;
                        continue; // or handle error
                    }

                    auto key = std::make_tuple(vIdx, vtIdx, vnIdx);
                    if (uniqueVertexMap.count(key) == 0)
                    {
                        Vertex vert;
                        vert.Position = positions[vIdx];
                        vert.TexCoords = texcoords[vtIdx];
                        vert.Normal = normals[vnIdx];
                        vertices.push_back(vert);
                        uniqueVertexMap[key] = static_cast<unsigned int>(vertices.size() - 1);
                    }
                    indices.push_back(uniqueVertexMap[key]);
                }
            }
            else if (prefix == "mtllib") // Material library
            {
                iss >> mtlFilePath;
                mtlFilePath = Texture::GetTexturePath(obj_model_filepath, mtlFilePath);
                Texture::LoadMTL(mtlFilePath, materials);
                std::cout << "mtlFilePath: " << mtlFilePath << std::endl;
            }
            else if (prefix == "usemtl") {
                std::cout << "usemtl entered" << std::endl;
                iss >> currentMaterialName;
                if (materials.find(currentMaterialName) != materials.end()) {
                    const Material& mat = materials[currentMaterialName];
                    shader.SetVec3("material.ambient", mat.ambient);
                    shader.SetVec3("material.diffuse", mat.diffuse);
                    shader.SetVec3("material.specular", mat.specular);
                    shader.SetFloat("material.shininess", mat.shininess);
                    std::cout << "material.ambient: " << mat.ambient.x << std::endl;
                    std::cout << "material.diffuse: " << mat.diffuse.x << std::endl;
                    std::cout << "material.specular: " << mat.specular.x << std::endl;
                    std::cout << "material.shininess: " << mat.shininess << std::endl;
                    
                    if (mat.diffuseMap != 0) {
                        Texture texture;
                        texture.id = mat.diffuseMap;
                        texture.type = "texture_diffuse";
                        textures.push_back(texture);
                    }
                }
            }
        }
        file.close();

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }

    void Renderer::Install(void)
    {
        meshes.push_back(Mesh(vertices, indices, textures));
        meshes[0].setupMesh();
    }

    void Renderer::Render(glm::vec3 position, glm::vec3 orientation)
    {

            meshes[0].Draw(shader, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.0f, 3.0f, 0.0f)));
            meshes[1].Draw(shader, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec3(1.0f, 1.0f, 1.0f)));

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