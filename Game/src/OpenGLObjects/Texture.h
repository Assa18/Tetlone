#pragma once

#include <glad/glad.h>

#include <string>

struct TextureData
{
	GLuint wrapS;
	GLuint wrapT;
	GLuint minFilter;
	GLuint magFilter;
	int32_t slotIndex;
};

class Texture
{
public:
	Texture();
	~Texture();

	inline uint32_t GetID() { return m_ID; }
	inline uint32_t GetIndex() { return m_Index; }

	void Bind() const;
	void Unbind() const;

	void Load(const std::string& path, TextureData& data);
private:
	uint32_t m_ID;

	int32_t m_Index;
};