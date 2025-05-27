#include "Lights.h"

namespace Render {
    Light::Light(LightType type) : type(type) {} // Construtor que define o tipo de luz

    void Light::Enable(bool enable) {
        this->enabled = enable; // Activa ou desactiva a luz
    }

    void Light::SetColor(const glm::vec3& color) {
        this->color = color; // Define a cor da luz
    }

    void Light::SetDirection(const glm::vec3& direction) {
        this->direction = direction; // Define a direcção da luz
    }

    void Light::SetPosition(const glm::vec3& position) {
        this->position = position; // Define a posição da luz
    }
    void Light::SetAmbient(const glm::vec3& ambient) {
        this->ambient = ambient; // Define o componente ambiente da luz
    }
    void Light::SetDiffuse(const glm::vec3& diffuse) {
        this->diffuse = diffuse; // Define o componente difuso da luz
    }
    void Light::SetSpecular(const glm::vec3& specular) {
        this->specular = specular; // Define o componente especular da luz
    }

    // Envia os valores da luz para o shader, dependendo do tipo de luz
    void Light::SetShaderLightValue(GLuint shader_program, glm::vec3 transformedPointLightPos, glm::vec3 transformedSpotLightPos, glm::vec3 transformedSpotLightDir) {
        std::string lightTypeString; // Nome do tipo de luz para usar nos uniforms do shader
        switch (type) {
            case LightType::Ambient:
                lightTypeString = "ambientLight"; 
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Activa/desactiva no shader
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".color").c_str()), 1, &color[0]); // Define a cor ambiente
                break;
            case LightType::Directional:
                lightTypeString = "directionalLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Activa/desactiva no shader
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".direction").c_str()), 1, &direction[0]); // Direcção da luz
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]); // Componente ambiente
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]); // Componente difuso
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]); // Componente especular
                break;
            case LightType::Point:
                lightTypeString = "pointLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Activa/desactiva no shader
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".position").c_str()), 1, &transformedPointLightPos[0]); // Posição da luz (transformada)
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]);
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kc_atenuation").c_str()), constant); // Atenuação constante
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kl_atenuation").c_str()), linear); // Atenuação linear
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kq_atenuation").c_str()), quadratic); // Atenuação quadrática
                break;
            case LightType::Spot:
                lightTypeString = "spotLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Activa/desactiva no shader
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".position").c_str()), 1, &transformedSpotLightPos[0]); // Posição da luz spot (transformada)
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".direction").c_str()), 1, &transformedSpotLightDir[0]); // Direcção da luz spot (transformada)
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]);
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kc_atenuation").c_str()), constant); // Atenuação constante
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kl_atenuation").c_str()), linear); // Atenuação linear
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kq_atenuation").c_str()), quadratic); // Atenuação quadrática
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".s_exponent").c_str()), exponent_val); // Expoente do spot
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".cutOff").c_str()), cutoff_angle); // Ângulo de corte interno
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".outerCutOff").c_str()), outr_cutoff_angle); // Ângulo de corte externo
                break;
        }
    }

    // Actualiza apenas o estado (enabled/disabled) da luz no shader
    void Light::UpdateShaderLightState(GLuint shader_program) {
        std::string lightTypeString;
        switch (type) {
            case LightType::Ambient:
                lightTypeString = "ambientLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Actualiza o estado da luz ambiente
                break;
            case LightType::Directional:
                lightTypeString = "directionalLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Actualiza o estado da luz direcional
                break;
            case LightType::Point:
                lightTypeString = "pointLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Actualiza o estado da luz pontual
                break;
            case LightType::Spot:
                lightTypeString = "spotLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); // Actualiza o estado da luz spot
                break;
        }
    }
}