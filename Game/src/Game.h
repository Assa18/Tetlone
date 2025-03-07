#pragma once

#include <glad/glad.h>

#include "Application/WindowFactory.h"
#include "GameRenderer.h"

class Game
{
public:
	Game();
	~Game();

	void Initialize();

	void Run();

	void Cleanup();

	inline static Window* GetWindow() { return s_Instance->m_Window; }
private:
	void Update();

	Window* m_Window;
	uint32_t m_Width, m_Height;

	GameRenderer m_Renderer;

	static Game* s_Instance;
};



void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);