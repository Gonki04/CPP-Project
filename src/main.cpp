#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <windows.h>
#include <vector>
#include <Camera.h>
#include "Minimap.h"
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

void init(void);

#define WIDTH 800
#define HEIGHT 600

GLfloat ZOOM = 10.0f;
GLfloat ANGLE = 0.0f;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset == 1) {
        ZOOM += fabs(ZOOM) * 0.1f; // Zoom in
    } else if (yoffset == -1) {
        ZOOM -= fabs(ZOOM) * 0.1f; // Zoom out
    }

    std::cout << "ZOOM = " << ZOOM << std::endl;
}

std::vector<glm::vec3> Load3DParallelepiped(void) {
	// 6 faces * 4 vértices por face  
	glm::vec3 point[6 * 4] = {
		// Frente
		glm::vec3(-2.5f, -0.1f,  1.25f),
		glm::vec3( 2.5f, -0.1f,  1.25f),
		glm::vec3( 2.5f,  0.1f,  1.25f),
		glm::vec3(-2.5f,  0.1f,  1.25f),
		// Trás
		glm::vec3(-2.5f, -0.1f, -1.25f),
		glm::vec3(-2.5f,  0.1f, -1.25f),
		glm::vec3( 2.5f,  0.1f, -1.25f),
		glm::vec3( 2.5f, -0.1f, -1.25f),
		// Direita
		glm::vec3( 2.5f, -0.1f,  1.25f),
		glm::vec3( 2.5f, -0.1f, -1.25f),
		glm::vec3( 2.5f,  0.1f, -1.25f),
		glm::vec3( 2.5f,  0.1f,  1.25f),
		// Esquerda
		glm::vec3(-2.5f, -0.1f,  1.25f),
		glm::vec3(-2.5f,  0.1f,  1.25f),
		glm::vec3(-2.5f,  0.1f, -1.25f),
		glm::vec3(-2.5f, -0.1f, -1.25f),
		// Cima
		glm::vec3(-2.5f,  0.1f,  1.25f),
		glm::vec3( 2.5f,  0.1f,  1.25f),
		glm::vec3( 2.5f,  0.1f, -1.25f),
		glm::vec3(-2.5f,  0.1f, -1.25f),
		// Baixo
		glm::vec3(-2.5f, -0.1f,  1.25f),
		glm::vec3(-2.5f, -0.1f, -1.25f),
		glm::vec3( 2.5f, -0.1f, -1.25f),
		glm::vec3( 2.5f, -0.1f,  1.25f)
	};
	

	std::vector<glm::vec3> ret;
	for (auto i : point)
		ret.push_back(i);

	return ret;
}

void display(std::vector<glm::vec3> points, glm::mat4 mvp) {
    // Remover glClear daqui!
    float* vertex_stream = static_cast<float*>(glm::value_ptr(points.front()));

    std::vector<glm::vec3> colors = {
        glm::vec3(0.0f, 0.3f, 0.0f), // Verde escuro
        glm::vec3(0.0f, 0.5f, 0.0f), // Verde médio
        glm::vec3(0.0f, 0.7f, 0.0f), // Verde erva
        glm::vec3(0.3f, 0.8f, 0.3f), // Verde claro
        glm::vec3(0.5f, 1.0f, 0.5f), // Verde menta
        glm::vec3(0.6f, 1.0f, 0.6f)  // Verde suave
    };
    
    glBegin(GL_QUADS);
    for (int nv = 0; nv < 6 * 4 * 3; nv += 3) {
        // Uma cor por face
        glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
        glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);
        // Cálculo das coordenadas de recorte
        glm::vec4 transformed_vertex = mvp * vertex;
        // Divisão de Perspetiva
        glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
        // Desenho do vértice
        glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
    }
    glEnd();
}

int main(void) {
    std::vector<glm::vec3> points = Load3DParallelepiped();

    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Pool Game", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    init();

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, ZOOM));

    // Associa a câmera ao "user pointer" da janela
    glfwSetWindowUserPointer(window, &camera);

    // Define o callback de scroll
    glfwSetScrollCallback(window, scrollCallback);

    // Projeção
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela apenas uma vez por frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Atualiza a matriz de visão e modelo
        glm::mat4 view = glm::lookAt(
            glm::vec3(8.0f, 1.0f, ZOOM),    // Posição da câmara no mundo
            glm::vec3(0.0f, 0.0f, -1.0f),    // Direção para a qual a câmara esta apontada
            glm::vec3(0.0f, 1.0f, 0.0f)        // Vector vertical
        );
        camera.model = glm::mat4(1.0f);
        //camera.model = glm::rotate(camera.model, ANGLE += 0.001f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
        glm::mat4 mvp = projection * camera.ViewMatrix * camera.model;

        display(points, mvp);

        // Chamar o minimapa
        drawMinimap(points, WIDTH, HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
}