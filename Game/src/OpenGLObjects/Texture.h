#pragma once

#include <glad/glad.h>

#include <string>

struct TextureData
{
	GLuint wrapS;
	GLuint wrapT;
	GLuint minFilter;
	GLuint magFilter;
};

class Texture
{
public:
	Texture();
	~Texture();

	void Bind() const;
	void Unbind() const;

	void Load(const std::string& path, TextureData& data);
private:
	unsigned int m_ID;
};