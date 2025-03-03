#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char* vShaderSorcePath, const char* fShaderSourcePath);
	~Shader();

	unsigned int getId() { return m_Id; }

	void bind();
	void unBind();

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetFloat3(const std::string& name, float x, float y, float z);
	void SetFloat3(const std::string& name, float values[]);
	void SetFloat4(const std::string& name, float x, float y, float z, float alpha);
	void SetFloat4(const std::string& name, float values[]);

	void SetMat4(const std::string& name, glm::mat4 matrix);
private:
	unsigned int m_Id;

	bool compileShader(unsigned int& program);
};