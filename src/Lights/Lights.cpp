#include "Lights.h"

namespace Render {
    Light::Light(LightType type) : type(type) {} 

    void Light::Enable(bool enable) {
        this->enabled = enable; 
    }

    void Light::SetColor(const glm::vec3& color) {
        this->color = color; 
    }

    void Light::SetDirection(const glm::vec3& direction) {
        this->direction = direction; 
    }

    void Light::SetPosition(const glm::vec3& position) {
        this->position = position; 
    }
    void Light::SetAmbient(const glm::vec3& ambient) {
        this->ambient = ambient; 
    }
    void Light::SetDiffuse(const glm::vec3& diffuse) {
        this->diffuse = diffuse; 
    }
    void Light::SetSpecular(const glm::vec3& specular) {
        this->specular = specular; 
    }

    
    void Light::SetShaderLightValue(GLuint shader_program, glm::vec3 transformedPointLightPos, glm::vec3 transformedSpotLightPos, glm::vec3 transformedSpotLightDir) {
        std::string lightTypeString; 
        switch (type) {
            case LightType::Ambient:
                lightTypeString = "ambientLight"; 
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".color").c_str()), 1, &color[0]); 
                break;
            case LightType::Directional:
                lightTypeString = "directionalLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".direction").c_str()), 1, &direction[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]); 
                break;
            case LightType::Point:
                lightTypeString = "pointLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".position").c_str()), 1, &transformedPointLightPos[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]);
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kc_atenuation").c_str()), constant); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kl_atenuation").c_str()), linear); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kq_atenuation").c_str()), quadratic); 
                break;
            case LightType::Spot:
                lightTypeString = "spotLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".position").c_str()), 1, &transformedSpotLightPos[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".direction").c_str()), 1, &transformedSpotLightDir[0]); 
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".ambient").c_str()), 1, &ambient[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".diffuse").c_str()), 1, &diffuse[0]);
                glProgramUniform3fv(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".specular").c_str()), 1, &specular[0]);
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kc_atenuation").c_str()), constant); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kl_atenuation").c_str()), linear); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".kq_atenuation").c_str()), quadratic); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".s_exponent").c_str()), exponent_val); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".cutOff").c_str()), cutoff_angle); 
                glProgramUniform1f(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".outerCutOff").c_str()), outr_cutoff_angle); 
                break;
        }
    }

    
    void Light::UpdateShaderLightState(GLuint shader_program) {
        std::string lightTypeString;
        switch (type) {
            case LightType::Ambient:
                lightTypeString = "ambientLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                break;
            case LightType::Directional:
                lightTypeString = "directionalLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                break;
            case LightType::Point:
                lightTypeString = "pointLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                break;
            case LightType::Spot:
                lightTypeString = "spotLight";
                glProgramUniform1i(shader_program, glGetUniformLocation(shader_program, (lightTypeString + ".enabled").c_str()), enabled); 
                break;
        }
    }
}