#pragma once

struct TextureData
{

};

class Texture
{
public:
	Texture();
	~Texture();

	void Load(TextureData& data);
private:
	unsigned int m_ID;
};