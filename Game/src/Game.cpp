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

		for (int i = -5; i < 5; i++)
		{
			for (int j = -10; j < 10; j++)
			{
				m_Renderer.Quad(glm::vec2(i * 0.2f, j * 0.2f), glm::vec2(0.2f, 0.2f), (std::abs(i) + std::abs(j))%2+1);
			}
		}

		m_Renderer.Draw(m_GameData.Camera);

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