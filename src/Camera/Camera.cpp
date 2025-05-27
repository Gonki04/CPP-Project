#include "Camera.h"

namespace Render {

// Construtor da câmara, define tamanho da janela e posição inicial
Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    // Cria a matriz de visualização (view matrix)
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);

    // Cria a matriz de projeção (projection matrix)
    glm::mat4 projection = glm::perspective(glm::radians(fov_), (float)width / (float)height, nearPlane, farPlane);

    // Combina as duas matrizes para obter a matriz de visualização + projeção
    glm::mat4 viewProjection = projection * view;

    // Envia as duas matrizes combinadas para o shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

// Ajusta FOV com base no movimento do scroll do rato (yoffset).
// yoffset é a quantidade de scroll (positivo para cima, negativo para baixo).
void Camera::HandleScroll(double yoffset) {

    if (fov_ >= 1.0f && fov_ <= 90.0f) // Se yoffset for positivo (scroll para cima), fov_ diminui (zoom in).
        fov_ -= yoffset;// Se yoffset for negativo (scroll para baixo), fov_ aumenta (zoom out).

    // Limita o FOV mínimo para 1.0 (zoom máximo).
    // Impede que o FOV fique muito pequeno ou negativo.
    if (fov_ <= 1.0f)
        fov_ = 1.0f;

    // Limita o FOV máximo para 90.0 graus (zoom mínimo).
    // Impede que o FOV fique excessivamente grande.
    if (fov_ >= 90.0f)
        fov_ = 90.0f;
}
}