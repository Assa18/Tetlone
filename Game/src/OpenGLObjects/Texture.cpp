#include "Texture.h"

#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture()
	:m_ID(0), m_Index(0)
{
}

Texture::~Texture()
{
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Load(const std::string& path, TextureData& data)
{
	m_Index = data.slotIndex;


	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.magFilter);

	stbi_set_flip_vertically_on_load(true);
	int width = 0, height = 0, nrChannels = 0;
	unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (!textureData)
	{
		std::cout << "Failed to load texture (" << path << ")!\n";
		exit(1);
	}

	if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else if (nrChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(textureData);
}