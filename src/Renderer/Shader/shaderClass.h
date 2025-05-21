#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	// CONSTRUCTOR THAT BULDS THE SHADER PROGRAM FROM 2 DIFFERENT SHADERS
	Shader();
	void LoadShader(const char* vertexFile, const char* fragmentFile);
	void Activate();
	void Delete();

	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetFloat(const std::string& name, float value) const;
	void SetInt(const std::string& name, int value) const;
private:
	// CHECKS IF DEIFFERENT SHADERS COMPILE AND LINK CORRECTLY
	void compileErrors(unsigned int shader, const char* type);
};


#endif