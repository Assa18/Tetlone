#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader()
	:m_Id(0)
{
}

Shader::~Shader()
{
	glDeleteProgram(m_Id);
}

void Shader::Initialize(const char* vShaderSourcePath, const char* fShaderSourcePath)
{
	std::ifstream vSource(vShaderSourcePath);
	std::ifstream fSource(fShaderSourcePath);

	if (!vSource || !fSource)
	{
		std::cout << "ERROR: failed to load input sjader files!\n";
		exit(1);
	}

	std::stringstream vertexstream;
	std::stringstream fragmentstream;

	std::string line;
	while (getline(vSource, line))
	{
		vertexstream << line << '\n';
	}

	while (getline(fSource, line))
	{
		fragmentstream << line << '\n';
	}

	std::string vShaderCode = vertexstream.str();
	std::string fShaderCode = fragmentstream.str();

	const char* vShaderSource = vShaderCode.c_str();
	const char* fShaderSource = fShaderCode.c_str();

	vSource.close();
	fSource.close();

	uint32_t vShader = 0, fShader = 0;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderSource, nullptr);
	if (!CompileShader(vShader))
	{
		std::cout << "ERROR: failed to compile vertex shader!\n";
		exit(1);
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderSource, nullptr);
	if (!CompileShader(fShader))
	{
		std::cout << "ERROR: failed to compile fragment shader!\n";
		exit(1);
	}

	m_Id = glCreateProgram();
	glAttachShader(m_Id, vShader);
	glAttachShader(m_Id, fShader);
	glLinkProgram(m_Id);

	glDetachShader(m_Id, vShader);
	glDetachShader(m_Id, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}


bool Shader::CompileShader(uint32_t& program)
{
	glCompileShader(program);

	int succes = 0;
	glGetShaderiv(program, GL_COMPILE_STATUS, &succes);

	if (!succes)
	{
		char infoLog[512];
		glGetShaderInfoLog(program, 512, nullptr, infoLog);
		std::cout << "ERROR: " << infoLog << "\n";
		glfwTerminate();
		return false;
	}
	return true;
}

void Shader::Bind()
{
	glUseProgram(m_Id);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::SetFloat3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(m_Id, name.c_str()), x, y, z);
}

void Shader::SetFloat3(const std::string& name, float values[])
{
	glUniform3f(glGetUniformLocation(m_Id, name.c_str()), values[0], values[1], values[2]);
}

void Shader::SetFloat4(const std::string& name, float x, float y, float z, float alpha)
{
	glUniform4f(glGetUniformLocation(m_Id, name.c_str()), x, y, z, alpha);
}

void Shader::SetFloat4(const std::string& name, float values[])
{
	glUniform4f(glGetUniformLocation(m_Id, name.c_str()), values[0], values[1], values[2], values[3]);
}

void Shader::SetVec3(const std::string& name, glm::vec3 vector)
{
	glUniform3f(glGetUniformLocation(m_Id, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::SetMat4(const std::string& name, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}