#pragma once
#include <cstdint>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Create(uint32_t width, uint32_t height);

	inline const uint32_t GetID() const { return m_ID; }
	inline const uint32_t GetTextureID() const { return m_TextureID; }

	void Resize(int32_t width, int32_t height);

	void BindVAO();

	void Bind();
	void Unbind();
private:
	uint32_t m_ID;
	uint32_t m_TextureID;
	uint32_t m_VAO, m_VBO;
};

