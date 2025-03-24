#pragma once

#include <glad/glad.h>

#include "Application/WindowFactory.h"
#include "GameRenderer.h"
#include "GameObjects/GameTile.h"

#include <vector>

struct GameData
{
	Camera2D Camera;

	std::map<std::pair<int, int>, GameTile> Tiles;

	std::vector<GameTile> MovingTiles;

	void Update()
	{
		Camera.OnUpdate(0.01f);
	}
};

class Game
{
public:
	Game();
	~Game();

	void Initialize();
	void Resize(uint32_t width, uint32_t height);

	void Run();
	void Cleanup();

	inline static Game* GetInstance() { return s_Instance; }
	inline static Window* GetWindow() { return s_Instance->m_Window; }
	inline GameData* GetGameData() { return &m_GameData; }
private:
	void Update();
	void CheckCollision();
	void SpawnNext();

	// returns the Y coord, of the most bottom full line, or -1
	void CheckFullLines(int& index, int& offset);
	void RemoveFullLines();
	void PullDownRest(int index, int offset);

	void Move(int dir);
private:
	Window* m_Window;
	uint32_t m_Width, m_Height;
	float m_Speed = 2.0f;
	float m_DeltaTime, m_LastFrame;

	GameRenderer m_Renderer;
	GameData m_GameData;

	static Game* s_Instance;

	glm::vec2 m_TileSize = glm::vec2(0.19f, 0.19f);

	bool m_IsFullLine[20] = { false };
};

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);