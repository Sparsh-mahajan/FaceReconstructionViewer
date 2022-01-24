#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <malloc.h>

struct ProgramShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
public:
	Shader(std::string& filePath);
	~Shader();
	ProgramShaderSource ParseShader();
	unsigned int CompileShader(unsigned int type, std::string& Source);
	unsigned int CreateShader(std::string& VertexShader, std::string& FragmentShader);
	void Bind() const;
	void UnBind() const;
	unsigned int GetProgramID() const;
	void SetUniformMatrix4fv(const char* name, glm::mat4& matrix);
	void SetUniform1i(const char* name, int value);
	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
};