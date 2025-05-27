#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Shader/shaderClass.h"

namespace Render{

class Camera
{
public:
    // Posição atual da câmara
    glm::vec3 Position;
    // Vetor de orientação da câmara (para onde olha)
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);

    // Largura e altura da janela
    int width;
    int height;

    // Campo de visão (FOV) inicial em graus
    float fov_ = 45.0f;

    // Construtor da câmara, define o tamanho da janela e a posição inicial
    Camera(int width, int height, glm::vec3 position);

    // Calcula e envia a matriz de projeção + view para o shader
    // FOVdeg: campo de visão em graus
    // nearPlane: plano próximo
    // farPlane: plano distante
    // shader: referência ao shader para onde será enviada a matriz
    // uniform: nome da variável uniform no shader
    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);

    // Lida com o scroll do rato para alterar o FOV (zoom in/out)
    void HandleScroll(double yoffset);

private:
    // Vetor "up" (para cima)
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
};
}
