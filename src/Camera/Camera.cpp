#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(fov_), (float)width / (float)height, nearPlane, farPlane);
    glm::mat4 viewProjection = projection * view;

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Camera::HandleScroll(double yoffset) {
    if (fov_ >= 1.0f && fov_ <= 90.0f)
        fov_ -= yoffset;
    if (fov_ <= 1.0f)
        fov_ = 1.0f;
    if (fov_ >= 90.0f)
        fov_ = 90.0f;
}


