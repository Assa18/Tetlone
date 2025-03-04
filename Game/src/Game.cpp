#include "Game.h"

#include <iostream>

Game::Game()
{
	m_Height = 720;
	m_Width = 1280;
	m_Window = nullptr;
}

Game::~Game()
{
}

void Game::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Tetlone", nullptr, nullptr);
	if (m_Window == nullptr)
	{
		std::cout << "ERROR: failed to crate grfw window!\n";
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: failed to load glad!\n";
		exit(1);
	}

	m_Renderer.Initialize();
}

void Game::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		Update();
		
		m_Renderer.Draw();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Game::Cleanup()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Game::Update()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}