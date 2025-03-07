#include "Game.h"

//might be replaced with a factory solution
#include "Application/WindowsInput.h"

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
	m_GameData = m_Renderer.GetGameData();
	m_GameData->Camera.OnResize((float)m_Width, (float)m_Height);
}

void Game::Run()
{
	while (!m_Window->IsClosed())
	{
		Update();
		
		m_Renderer.Draw();
		m_GameData->Update();

		m_Window->Update();
	}
}

void Game::Cleanup()
{
	delete m_Window;
}

void Game::Update()
{
	if (Input::IsKeyPressed(GAME_KEY_ESCAPE))
		m_Window->SetClosed(true);
}