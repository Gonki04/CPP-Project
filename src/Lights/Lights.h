#pragma once
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Render {

    enum class LightType {
        Ambient,
        Directional,
        Point,
        Spot
    };

    class Light {
    public:
        LightType type;
        bool enabled = true;
        glm::vec3 color = glm::vec3(0.1f); // Soft gray ambient

        //Attenuation values
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float exponent_val = 32.0f;
        float cutoff_angle = glm::cos(glm::radians(12.5f));
        float outr_cutoff_angle = glm::cos(glm::radians(17.5f));

        //Directional Light
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 ambient = glm::vec3(0.1f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.0f);

        //Point Light
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        Light(LightType type);
        void Enable(bool enable);
        void SetColor(const glm::vec3& color);
        void SetDirection(const glm::vec3& direction);
        void SetPosition(const glm::vec3& position);
		void SetAmbient(const glm::vec3& ambient);
		void SetDiffuse(const glm::vec3& diffuse);
		void SetSpecular(const glm::vec3& specular);

        void SetShaderLightValue(GLuint shader_program, glm::vec3 transformedPointLightPos, glm::vec3 transformedSpotLightPos, glm::vec3 transformedSpotLightDir);

        void UpdateShaderLightState(GLuint shader_program);
    };
}
