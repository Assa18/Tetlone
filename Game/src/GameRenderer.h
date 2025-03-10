#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "OpenGLObjects/Shader.h"
#include "OpenGlObjects/Camera2D.h"

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

	unsigned int VAO = 0, VBO = 0, EBO = 0;

	uint32_t textureSlots[16];
	uint32_t whiteTexture;
};

struct GameData
{
	Camera2D Camera;

	std::map<std::pair<int, int>, GameTile> m_Tiles;


	void Update()
	{
		Camera.OnUpdate(0.01f);
	}
};

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialize();

	void Destroy();

	void BeginRender();
	void Draw(const Camera2D& cam);

	void Quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
	void Quad(const glm::vec2& pos, const glm::vec2& size, int texId);
private:
	Shader m_Shader;
};