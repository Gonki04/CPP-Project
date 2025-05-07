#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <windows.h>
#include <vector>
#include <Camera.h>
#include "Minimap.h"
#include "Camera.h"

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

std::vector<glm::vec3> Load3DParallelepiped()
{
	std::vector<glm::vec3> points = {
		// Front
		glm::vec3(-2.5f, -0.1f, 1.25f), glm::vec3(2.5f, -0.1f, 1.25f),
		glm::vec3(2.5f, 0.1f, 1.25f), glm::vec3(-2.5f, 0.1f, 1.25f),
		// Back
		glm::vec3(-2.5f, -0.1f, -1.25f), glm::vec3(-2.5f, 0.1f, -1.25f),
		glm::vec3(2.5f, 0.1f, -1.25f), glm::vec3(2.5f, -0.1f, -1.25f),
		// Right
		glm::vec3(2.5f, -0.1f, 1.25f), glm::vec3(2.5f, -0.1f, -1.25f),
		glm::vec3(2.5f, 0.1f, -1.25f), glm::vec3(2.5f, 0.1f, 1.25f),
		// Left
		glm::vec3(-2.5f, -0.1f, 1.25f), glm::vec3(-2.5f, 0.1f, 1.25f),
		glm::vec3(-2.5f, 0.1f, -1.25f), glm::vec3(-2.5f, -0.1f, -1.25f),
		// Top
		glm::vec3(-2.5f, 0.1f, 1.25f), glm::vec3(2.5f, 0.1f, 1.25f),
		glm::vec3(2.5f, 0.1f, -1.25f), glm::vec3(-2.5f, 0.1f, -1.25f),
		// Bottom
		glm::vec3(-2.5f, -0.1f, 1.25f), glm::vec3(-2.5f, -0.1f, -1.25f),
		glm::vec3(2.5f, -0.1f, -1.25f), glm::vec3(2.5f, -0.1f, 1.25f)};
	return points;
}

void Display(const std::vector<glm::vec3> &points, const glm::mat4 &mvp)
{
	const float *vertex_stream = glm::value_ptr(points.front());

	std::vector<glm::vec3> colors = {
		glm::vec3(0.0f, 0.3f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.3f, 0.8f, 0.3f),
		glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.6f, 1.0f, 0.6f)};

	glBegin(GL_QUADS);
	for (int nv = 0; nv < 6 * 4 * 3; nv += 3)
	{
		glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g,
				  colors[nv / (4 * 3)].b);
		glm::vec4 vertex =
			glm::vec4(vertex_stream[nv], vertex_stream[nv + 1],
					  vertex_stream[nv + 2], 1.0f);
		glm::vec4 transformed_vertex = mvp * vertex;
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		glVertex3f(normalized_vertex.x, normalized_vertex.y,
				   normalized_vertex.z);
	}
	glEnd();
}

void Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
}

int main()
{
	std::vector<glm::vec3> points = Load3DParallelepiped();

	if (!glfwInit())
		return -1;

	GLFWwindow *window =
		glfwCreateWindow(kWidth, kHeight, "Pool Game", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	Init();

	Camera camera(kWidth, kHeight, glm::vec3(0.0f, 0.0f, kInitialZoom));
	glfwSetWindowUserPointer(window, &camera);
	glfwSetScrollCallback(window, ScrollCallback);

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f), static_cast<float>(kWidth) / kHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela apenas uma vez por frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view_matrix = glm::lookAt(
			glm::vec3(3.0f, 3.0f, camera.zoom()),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model_matrix = glm::mat4(1.0f);
		//model_matrix = glm::rotate(model_matrix, kAngle += kRotationSpeed,
								   //glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
		glm::mat4 mvp = projection * view_matrix * model_matrix;

        Display(points, mvp);

        // Chamar o minimapa
        drawMinimap(points, kWidth, kHeight);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}