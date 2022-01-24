#pragma once

#include "Includes/Shader.h"

Shader::Shader(std::string& filePath) :
	m_Filepath(filePath), m_RendererID(0)
{
	ProgramShaderSource Source = ParseShader();
	//std::cout << Source.VertexSource << std::endl;
	//std::cout<<Source.FragmentSource<<std::endl;

	m_RendererID = CreateShader(Source.VertexSource, Source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

ProgramShaderSource Shader::ParseShader()
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];

	std::ifstream stream(m_Filepath);
	std::string line;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			//add any code of their respective shader to the respective string
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str() ,ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, std::string& Source)
{
	unsigned int id = glCreateShader(type);
	const char* src = Source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int Result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)(_alloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex " : "Fragment ") << "shader\n";
		std::cout << message << "\n";

		glDeleteShader(id);

		return 0;
	}
	return id;
}
unsigned int Shader::CreateShader(std::string& VertexShader, std::string& FragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	int Result;
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)(_alloca(length * sizeof(char)));
		glGetProgramInfoLog(program, length, &length, message);

		std::cout << "Failed to Link Program ";
		std::cout << message << "\n";
	}

	return program;
}
void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}
void Shader::UnBind() const 
{
	glUseProgram(0);
}
unsigned int Shader::GetProgramID() const
{
	return m_RendererID;
}
void Shader::SetUniformMatrix4fv(const char* name, glm::mat4& matrix)
{
	unsigned int uniformLoc = glGetUniformLocation(m_RendererID,name);
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::SetUniform1i(const char* name, int value)
{
	unsigned int uniformLoc = glGetUniformLocation(m_RendererID, name);
	glUniform1i(uniformLoc, value);
}
void Shader::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	unsigned int uniformLoc = glGetUniformLocation(m_RendererID, name);
	glUniform4f(uniformLoc, v0, v1, v2, v3);
}