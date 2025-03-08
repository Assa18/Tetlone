#include "Game.h"

//might be replaced with a factory solution
#include "Application/WindowsInput.h"

#include <iostream>

Game::Game()
{
	m_Height = 800;
	m_Width = 800;
	m_Window = nullptr;
}

Game::~Game()
{
}

Game* Game::s_Instance = nullptr;
void Game::Initialize()
{
	s_Instance = this;

	m_Window = Windowfactory::GetWindow(m_Width, m_Height);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: failed to load glad!\n";
		exit(1);
	}

	m_Renderer.Initialize();
}

void Game::Run()
{
	while (!m_Window->IsClosed())
	{
		Update();
		
		m_GameData.Update();

		m_Renderer.BeginRender();

		m_Renderer.Quad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_Renderer.Quad(glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_Renderer.EndRender();

		m_Window->Update();
	}
}

void Game::Cleanup()
{
	m_Renderer.Destroy();
	delete m_Window;
}

void Game::Update()
{
	if (Input::IsKeyPressed(GAME_KEY_ESCAPE))
		m_Window->SetClosed(true);
}