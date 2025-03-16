#include "Game.h"

//might be replaced with a factory solution
#include "Application/WindowsInput.h"

#include <iostream>
#include <time.h>

static int shapeDirs[] = {
	 0, 0, 0, 1, 0, 2, 0, 3, // I
	 0, 0, 1, 0, 1, 1, 1, 2, // J
	 0, 0, 0, 1, 0, 2, 1, 0, // L
	 0, 0, 1, 0, 0, 1, 1, 1, // O
	 0, 0, 1, 0, 1, 1, 2, 1, // S
	 0, 1, 1, 1, 1, 0, 2, 1, // T
	 0, 1, 1, 1, 1, 0, 2, 0  // Z
};

Game::Game()
{
	m_Height = 800;
	m_Width = 800;
	m_Window = nullptr;
	m_DeltaTime = 0.0f;
	m_LastFrame = 0.0f;
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

	SpawnNext();
}

void Game::Run()
{
	while (!m_Window->IsClosed())
	{
		float curFrame = glfwGetTime();
		m_DeltaTime = curFrame - m_LastFrame;
		m_LastFrame = curFrame;
		Update();

		//m_GameData.Update();

		m_Renderer.BeginRender();


		// background
		m_Renderer.Quad(glm::vec2(-1.0f, -2.0f), glm::vec2(2.0f, 4.0f), 3);
		
		for (auto& tile : m_GameData.Tiles)
		{
			m_Renderer.Quad(glm::vec2(tile.second.X * 0.2f + 0.005f, tile.second.Y * 0.2f + 0.005f), m_TileSize, tile.second.Color);
		}

		for (auto& tile : m_GameData.MovingTiles)
		{
			m_Renderer.Quad(glm::vec2(tile.X * 0.2f + 0.005f, tile.Y * 0.2f + 0.005f), m_TileSize, tile.Color);
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

	if (Input::IsKeyJustPressed(GAME_KEY_A))
		Move(-1);

	if (Input::IsKeyJustPressed(GAME_KEY_D))
		Move(1);

	for (int i = 0; i < m_GameData.MovingTiles.size(); i++)
	{
		m_GameData.MovingTiles[i].Y -= m_Speed * m_DeltaTime;
	}

	CheckCollision();
}

void Game::Resize(uint32_t width, uint32_t height)
{
	m_Renderer.OnResize(width, height);
}

void Game::Move(int dir)
{
	bool canMove = true;
	for (const auto& tile : m_GameData.MovingTiles)
	{
		if (tile.X + dir < -5 || tile.X + dir>4 || 
			m_GameData.Tiles.find(std::make_pair<int, int>(std::trunc(tile.X)+dir, std::trunc(tile.Y))) != m_GameData.Tiles.end())
		{
			canMove = false;
			break;
		}
	}

	if (canMove)
	{
		for (auto& tile : m_GameData.MovingTiles)
		{
			tile.X += dir;
		}
	}
}

void Game::CheckCollision()
{
	if (m_GameData.MovingTiles.size() == 0)
		return;

	bool shouldStop = false;
	for (const auto& tile : m_GameData.MovingTiles)
	{
		if (std::abs(tile.Y + 10) < 0.05f)
		{
			shouldStop = true;
			break;
		}

		for (int y = 9; y >= -10; y--)
		{
			if (m_GameData.Tiles.find(std::make_pair<int,int>(std::trunc(tile.X), (float)y)) != m_GameData.Tiles.end() &&
				std::abs(tile.Y - y) < 0.05f)
			{
				shouldStop = true;
				break;
			}
		}
	}

	if (shouldStop)
	{
		for (const auto& tile : m_GameData.MovingTiles)
		{
			m_GameData.Tiles[std::make_pair<int, int>(std::trunc(tile.X), std::trunc(tile.Y))] = tile;
		}

		m_GameData.MovingTiles.clear();
		SpawnNext();
	}
}

void Game::SpawnNext()
{
	glm::vec4 color = glm::vec4((float)(rand() % 255) / (float)254, (float)(rand() % 255) / (float)254, (float)(rand() % 255) / (float)254, 1.0f);
	m_GameData.MovingTiles.reserve(4);

	int index = rand() % 7;
	for (int i = (index * 8); i < (index + 1) * 8; i += 2)
	{
		m_GameData.MovingTiles.emplace_back(shapeDirs[i], 10 + shapeDirs[i + 1], color);
	}
}