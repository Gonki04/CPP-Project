#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    bool Init();
    void Display();

private:
    GLFWwindow* window = nullptr;
    int width, height;
    std::string title;

    void SetupOpenGL();
    void PrintSystemInfo();
};