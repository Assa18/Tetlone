#pragma once

#include <glad/glad.h>

#include "Application/WindowFactory.h"
#include "GameRenderer.h"
#include "GameObjects/GameTile.h"

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

	Window* m_Window;
	uint32_t m_Width, m_Height;

	GameRenderer m_Renderer;
	GameData m_GameData;

	static Game* s_Instance;
};



void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);