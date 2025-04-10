#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "OpenGLObjects/Shader.h"
#include "OpenGLObjects/Texture.h"
#include "OpenGlObjects/Camera2D.h"
#include "OpenGLObjects/FrameBuffer.h"
#include "OpenGLObjects/Character.h"

#include "GameObjects/GameTile.h"

#include <map>

struct Vertex
{
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 texCoords;
	float texIndex;
};

struct RenderData
{
	uint32_t maxTileCount;
	uint32_t maxVertexCount;
	uint32_t maxIndexCount;

	uint32_t* indices;
	uint32_t indicesCount;

	Vertex* vertices;
	Vertex* vertexIndexPtr;

	uint32_t VAO = 0, VBO = 0, EBO = 0;

	uint32_t textureSlots[16];
	uint32_t whiteTexture;
};

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialize();
	void OnResize(uint32_t width, uint32_t height);

	void Destroy();

	void BeginRender();
	void Draw(const Camera2D& cam);

	void Quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
	void Quad(const glm::vec2& pos, const glm::vec2& size, int texId);
	void Text(const std::string& text, const glm::vec2& pos, const glm::vec4& color, float height);

private:
	void LoadFontMap(const char* path);

private:
	Shader m_Shader;
	Shader m_ScreenShader;

	FrameBuffer m_FrameBuffer;

	std::map<std::string, Texture> m_Textures;
	std::map<char, Character> m_Characters;
	uint32_t m_FontTextureAtlasID;
};