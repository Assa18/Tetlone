#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "OpenGLObjects/Shader.h"
#include "OpenGlObjects/Camera2D.h"

#include "GameObjects/GameTile.h"

#include <unordered_map>

struct GameData
{
	Camera2D Camera;

	//std::unordered_map<std::pair<int, int>, GameTile> m_Tiles;


	void Update()
	{
		Camera.OnUpdate(0.01f);
	}
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	float texIndex;
};

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialize();
	void Draw();

	void Destroy();

	inline GameData* GetGameData() { return &m_GameData; }
private:
	Shader m_Shader;
	GameData m_GameData;

	Vertex* m_Vertices;
	uint32_t m_VertexIndex;
	uint32_t m_MaxTileCount;
};