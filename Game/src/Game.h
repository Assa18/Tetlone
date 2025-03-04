#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameRenderer.h"

class Game
{
public:
	Game();
	~Game();

	void Initialize();

	void Run();

	void Cleanup();
private:
	void Update();

	GLFWwindow* m_Window;
	uint32_t m_Width, m_Height;

	GameRenderer m_Renderer;
};

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);