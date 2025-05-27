#pragma once

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

std::string get_file_contents(const char* filename); // Funçao para ler o conteudo de um ficheiro

namespace Render {

class Shader
{
public:
	GLuint ID; // ID do shader program
	Shader(); // Construtor que cria o shader program e gera um ID
	void LoadShader(const char* vertexFile, const char* fragmentFile); // Carrega os shaders a partir dos ficheiros de vertex e fragment
	void Activate(); // Ativa o shader program
	void Delete(); // Apaga o shader program

	void SetMat4(const std::string& name, const glm::mat4& mat) const; // Define uma matriz 4x4 no shader
	void SetVec3(const std::string& name, const glm::vec3& vec) const; // Define um vetor 3D no shader
	void SetFloat(const std::string& name, float value) const; // Define um valor float no shader
	void SetInt(const std::string& name, int value) const; // Define um valor inteiro no shader
private:
	void compileErrors(unsigned int shader, const char* type); // Função para verificar erros de compilação dos shaders
};


}