#pragma once

#include <glad/glad.h>

#include "Application/WindowFactory.h"
#include "GameRenderer.h"
#include "GameObjects/GameTile.h"

#include <vector>

enum class GameStates {
	MENU,
	PLAYING,
	TOP_SCORES,
	ABOUT,
	SETTINGS
};

struct GameLogicData
{
	int Points = 0;
	int MenuSelectedIndex = 0;
	int HighScores[10] = { 0 };
	int FullLineCount = 0;
	bool IsSoundEnabled = true;
};

struct GameData
{
	Camera2D Camera;

	std::map<std::pair<int, int>, GameTile> Tiles;

	std::vector<GameTile> MovingTiles;
	std::vector<GameTile> NextTiles;

	float ShakeTime = 0.0f;

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

	void UpdatePlay();
	void UpdateMenu();
	void UpdateHighScores();
	void UpdateAbout();
	void UpdateSettings();

	void CheckCollision();
	void SpawnNext();

	void CheckFullLines(int& index, int& offset);
	void RemoveFullLines();
	void PullDownRest(int index, int offset);
	void RotateMovingObject();
	bool CheckRotated(std::pair<int, float>* positions);

	void Move(int dir);

	void AddPointsToHighScores(int point);
	void InitHighScores();
private:
	Window* m_Window;
	uint32_t m_Width, m_Height;
	float m_Speed = 2.0f;
	float m_DeltaTime, m_LastFrame;

	GameRenderer m_Renderer;
	GameData m_GameData;
	GameLogicData m_GameLogicData;

	static Game* s_Instance;

	glm::vec2 m_TileSize = glm::vec2(0.19f, 0.19f);

	bool m_IsFullLine[20] = { false };
	GameStates m_State = GameStates::MENU;
};

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);