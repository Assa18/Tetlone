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
		m_Renderer.Quad(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 4.0f), 3);
		
		for (auto& tile : m_GameData.Tiles)
		{
			m_Renderer.Quad(glm::vec2(tile.first.first * 0.2f + 0.005f, tile.first.second * 0.2f + 0.005f), m_TileSize, tile.second.Color);
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

	if (Input::IsKeyJustPressed(GAME_KEY_A) || Input::IsKeyJustPressed(GAME_KEY_LEFT))
		Move(-1);

	if (Input::IsKeyJustPressed(GAME_KEY_D) || Input::IsKeyJustPressed(GAME_KEY_RIGHT))
		Move(1);

	if (Input::IsKeyJustPressed(GAME_KEY_S) || Input::IsKeyJustPressed(GAME_KEY_DOWN))
		m_Speed += 4.0f;

	if (Input::IsKeyJustPressed(GAME_KEY_R))
		RotateMovingObject();

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
		if (tile.X + dir < 0 || tile.X + dir>9 || 
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
		if (std::abs(tile.Y) < 0.05f)
		{
			shouldStop = true;
			break;
		}

		for (int y = 19; y >= 0; y--)
		{
			if (m_GameData.Tiles.find(std::make_pair<int,int>(std::trunc(tile.X), (float)y)) != m_GameData.Tiles.end() &&
				std::abs(tile.Y - (y + 1)) < 0.05f)
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
		
		int index = 0, offset = 0;
		CheckFullLines(index, offset);
		if (index != -1)
		{
			RemoveFullLines();
			PullDownRest(index, offset);
		}
		
		m_Speed = 2.0f;
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
		m_GameData.MovingTiles.emplace_back(index, 5 + shapeDirs[i], 20 + shapeDirs[i + 1], color);
	}
}

void Game::CheckFullLines(int& index, int& offset)
{
	index = -1;
	offset = 0;

	for (int i = 0; i < 20; i++)
	{
		bool isFull = true;
		for (int j = 0; j < 10; j++)
		{
			if (m_GameData.Tiles.find(std::make_pair<int, int>((float)j, (float)i)) == m_GameData.Tiles.end())
			{
				isFull = false;
				break;
			}
		}

		if (isFull)
		{
			m_IsFullLine[i] = true;
			if (index == -1)
				index = i;
			offset++;
		}
	}
}

void Game::RemoveFullLines()
{
	for (int i = 0; i < 20; i++)
	{
		if (m_IsFullLine[i])
		{
			m_IsFullLine[i] = false;
			for (int j = 0; j < 10; j++)
			{
				m_GameData.Tiles.erase(std::make_pair<int, int>((float)j, (float)i));
			}
		}
	}
}

void Game::PullDownRest(int index, int offset)
{
	for (int i = index + 1; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (m_GameData.Tiles.find(std::make_pair<int, int>((float)j, (float)i)) != m_GameData.Tiles.end())
			{
				m_GameData.Tiles[std::make_pair<int, int>((float)j, (float)i - offset)] = m_GameData.Tiles[std::make_pair<int, int>((float)j, (float)i)];

				m_GameData.Tiles.erase(std::make_pair<int, int>((float)j, (float)i));
			}
		}
	}
}

void Game::RotateMovingObject()
{
	std::pair<int, float> newPositions[4];
	switch (m_GameData.MovingTiles[0].type)
	{
	case 0: // I
		break;
	case 1: // J
		break;
	case 2: // L
		break;
	case 3: // O
		break;
	case 4: // S
		break;
	case 5: // T
		break;
	case 6: // Z
		break;
	default:
		break;
	}

	if (CheckRotated(newPositions))
	{
		m_GameData.MovingTiles[0].X = newPositions[0].first;
		m_GameData.MovingTiles[0].Y = newPositions[0].second;
		m_GameData.MovingTiles[1].X = newPositions[1].first;
		m_GameData.MovingTiles[1].Y = newPositions[1].second;
		m_GameData.MovingTiles[2].X = newPositions[2].first;
		m_GameData.MovingTiles[2].Y = newPositions[2].second;
		m_GameData.MovingTiles[3].X = newPositions[3].first;
		m_GameData.MovingTiles[3].Y = newPositions[3].second;
	}
}

bool Game::CheckRotated(std::pair<int, float>* positions)
{
	bool correct = true;

	for (int i = 0; i < 4; i++)
	{
		positions[i].second = std::floor(positions[i].second);
		if (m_GameData.Tiles.find(positions[i]) == m_GameData.Tiles.end())
		{
			correct = false;
			break;
		}
	}

	return correct;
}