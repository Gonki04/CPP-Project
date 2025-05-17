//#include <Camera/Camera.h>
//#include "Minimap/Minimap.h"
#include <Renderer/Renderer.h>
#include <Windows.h>
#include <iostream>

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

namespace
{
	constexpr int kWidth = 800;
	constexpr int kHeight = 600;
	constexpr float kInitialZoom = 10.0f;
	constexpr float kRotationSpeed = 0.001f;
	GLfloat kAngle = 0.0f;
} // namespace


int main() {
    try {
        Render::Renderer renderer(kWidth, kHeight, "OpenGL Application");
        if (!renderer.Init()) {
            std::cerr << "Renderer initialization failed!" << std::endl;
            return -1;
        }

        std::cout << "Application started successfully" << std::endl;

        renderer.Display();
        return 0;
    }


    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
}