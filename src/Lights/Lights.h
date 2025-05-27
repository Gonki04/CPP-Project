#pragma once
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Render {

    // Enum para definir os tipos de luzes
    enum class LightType {
        Ambient,
        Directional,
        Point,
        Spot
    };

    class Light {
    public:
        LightType type; // Tipo de luz (Ambient, Directional, Point, Spot)
        bool enabled = true; // Estado da luz (ativa ou desativada)
        glm::vec3 color = glm::vec3(0.1f); // Cor da luz

        // Valores de atenuação para luzes do tipo Point
        float constant = 1.0f; // Atenuação constante
        float linear = 0.09f; // Atenuação linear
        float quadratic = 0.032f; // Atenuação quadrática
        float exponent_val = 32.0f; // Exponente para o brilho especular
        float cutoff_angle = glm::cos(glm::radians(12.5f)); // Ângulo de corte para luzes do tipo Spot
        float outr_cutoff_angle = glm::cos(glm::radians(17.5f)); // Ângulo de corte externo para luzes do tipo Spot

        // Luz Direcional
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // Direção da luz direcional
        glm::vec3 ambient = glm::vec3(0.1f); // Componente ambiente
        glm::vec3 diffuse = glm::vec3(0.8f); // Componente difusa
        glm::vec3 specular = glm::vec3(1.0f); // Componente especular

        // Luz point
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Posição da luz point

        Light(LightType type); // Construtor que define o tipo de luz
        void Enable(bool enable); // Activa ou desactiva a luz
        void SetColor(const glm::vec3& color); // Define a cor da luz 
        void SetDirection(const glm::vec3& direction); // Define a direcção da luz
        void SetPosition(const glm::vec3& position); // Define a posição da luz point
		void SetAmbient(const glm::vec3& ambient); // Define o componente ambiente da luz 
		void SetDiffuse(const glm::vec3& diffuse); // Define o componente difuso da luz
		void SetSpecular(const glm::vec3& specular); // Define o componente especular da luz

        void SetShaderLightValue(GLuint shader_program, glm::vec3 transformedPointLightPos, glm::vec3 transformedSpotLightPos, glm::vec3 transformedSpotLightDir); // Envia os valores da luz para o shader, dependendo do tipo de luz

        void UpdateShaderLightState(GLuint shader_program); // Atualiza o estado da luz no shader
    };
}
