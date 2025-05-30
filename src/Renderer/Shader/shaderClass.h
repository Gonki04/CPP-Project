#pragma once

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

std::string get_file_contents(const char* filename); 

namespace Render {

class Shader
{
public:
	GLuint ID; 
	Shader(); 
	void LoadShader(const char* vertexFile, const char* fragmentFile); 
	void Activate(); 
	void Delete(); 

	void SetMat4(const std::string& name, const glm::mat4& mat) const; 
	void SetVec3(const std::string& name, const glm::vec3& vec) const; 
	void SetFloat(const std::string& name, float value) const; 
	void SetInt(const std::string& name, int value) const; 
private:
	void compileErrors(unsigned int shader, const char* type); 
};


}