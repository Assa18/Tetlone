#include "Game.h"

//might be replaced with a factory solution
#include "Application/WindowsInput.h"

#include <iostream>
#include <time.h>
#include <fstream>

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

	InitHighScores();
	m_Renderer.Initialize();
}

void Game::Run()
{
	while (!m_Window->IsClosed())
	{
		float curFrame = glfwGetTime();
		m_DeltaTime = curFrame - m_LastFrame;
		m_LastFrame = curFrame;

		Update();
		m_Renderer.BeginRender();

		// background
		m_Renderer.Quad(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 4.0f), 2);
		m_Renderer.Text("Points: " + std::to_string(m_GameLogicData.Points), { -1.95f, 1.75f }, { 1.0f,0.0f,0.0f,1.0f }, 0.0035f);
		m_Renderer.Text("Next: ", { 1.20f, 1.75f }, { 1.0f,0.0f,0.0f,1.0f }, 0.0035f);

		m_Renderer.Quad({ 2.12f, 2.7f }, { 0.75f,0.85f }, { 0.15f,0.15f,0.15f,1.0f });
		
		for (auto& tile : m_GameData.Tiles)
		{
			m_Renderer.Quad(glm::vec2(tile.first.first * 0.2f + 0.005f, tile.first.second * 0.2f + 0.005f), m_TileSize, tile.second.Color);
		}

		switch (m_State)
		{
		case GameStates::MENU:
			m_Renderer.Quad({ -2.0f, 0.0f }, { 5.0f,4.0f }, { 0.55f,0.55f,0.55f,0.5f });
			UpdateMenu();
			break;
		case GameStates::PLAYING:
			for (auto& tile : m_GameData.NextTiles)
			{
				m_Renderer.Quad(glm::vec2(1.85f + tile.X * 0.1f + 0.005f, 1.0f + tile.Y * 0.1f + 0.005f), m_TileSize * 0.5f, tile.Color);
			}

			for (auto& tile : m_GameData.MovingTiles)
			{
				m_Renderer.Quad(glm::vec2(tile.X * 0.2f + 0.005f, tile.Y * 0.2f + 0.005f), m_TileSize, tile.Color);
			}
			UpdatePlay();
			break;
		case GameStates::TOP_SCORES:
			m_Renderer.Quad({ -2.0f, 0.0f }, { 5.0f,4.0f }, { 0.55f,0.55f,0.55f,0.5f });
			UpdateHighScores();
			break;
		case GameStates::SETTINGS:
			m_Renderer.Quad({ -2.0f, 0.0f }, { 5.0f,4.0f }, { 0.55f,0.55f,0.55f,0.5f });
			break;
		case GameStates::ABOUT:
			m_Renderer.Quad({ -2.0f, 0.0f }, { 5.0f,4.0f }, { 0.55f,0.55f,0.55f,0.5f });
			break;
		default:
			break;
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

void Game::UpdatePlay()
{
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

void Game::UpdateMenu()
{
	if (Input::IsKeyJustPressed(GAME_KEY_ENTER))
	{
		switch (m_GameLogicData.MenuSelectedIndex)
		{
		case 0:
			m_State = GameStates::PLAYING;
			SpawnNext();
			SpawnNext();
			break;
		case 1:
			m_State = GameStates::TOP_SCORES;
			break;
		case 2:
			m_State = GameStates::ABOUT;
			break;
		case 3:
			m_State = GameStates::SETTINGS;
			break;
		default:
			break;
		}
	}

	if (Input::IsKeyJustPressed(GAME_KEY_DOWN))
	{
		m_GameLogicData.MenuSelectedIndex = std::min(3, ++m_GameLogicData.MenuSelectedIndex);
	}

	if (Input::IsKeyJustPressed(GAME_KEY_UP))
	{
		m_GameLogicData.MenuSelectedIndex = std::max(0, --m_GameLogicData.MenuSelectedIndex);
	}

	m_Renderer.Text("Menu", { -0.45f, 1.75f }, { 1.0f,0.0f,0.0f,1.0f }, 0.005f);
	m_Renderer.Text("Play", { -0.45f, 1.45f }, { 1.0f,0.0f,0.0f,1.0f }, 0.004f);
	m_Renderer.Text("High scores", { -0.45f, 1.25f }, { 1.0f,0.0f,0.0f,1.0f }, 0.004f);
	m_Renderer.Text("About", { -0.45f, 1.05f }, { 1.0f,0.0f,0.0f,1.0f }, 0.004f);
	m_Renderer.Text("Settings", { -0.45f, 0.85f }, { 1.0f,0.0f,0.0f,1.0f }, 0.004f);

	m_Renderer.Text("<-", { 0.65f, 1.45f - m_GameLogicData.MenuSelectedIndex * 0.2f}, { 1.0f,0.0f,0.0f,1.0f }, 0.005);
}


void Game::UpdateHighScores()
{
	if (Input::IsKeyJustPressed(GAME_KEY_BACKSPACE))
		m_State = GameStates::MENU;

	m_Renderer.Text("High scores:", { -0.45f, 1.75f }, { 1.0f,0.0f,0.0f,1.0f }, 0.005f);
	for (int i = 0; i < 10; i++)
	{
		m_Renderer.Text(std::to_string(i+1) + ": " + std::to_string(m_GameLogicData.HighScores[i]), 
			{-0.45f, 1.45f - i * 0.2f}, {1.0f,0.0f,0.0f,1.0f}, 0.004f);
	}
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
		m_GameLogicData.Points++;
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

		for (int i = 0; i < 10; i++)
		{
			if (m_GameData.Tiles.find(std::make_pair<int, int>((float)i, 19)) != m_GameData.Tiles.end())
			{
				m_GameData.Tiles.clear();
				m_State = GameStates::MENU;
				AddPointsToHighScores(m_GameLogicData.Points);
				m_GameLogicData.Points = 0;
				break;
			}
		}

		SpawnNext();
	}
}

void Game::SpawnNext()
{
	m_GameData.MovingTiles = m_GameData.NextTiles;
	m_GameData.NextTiles.clear();
	glm::vec4 color = glm::vec4((float)(rand() % 255) / (float)254, (float)(rand() % 255) / (float)254, (float)(rand() % 255) / (float)254, 1.0f);
	m_GameData.NextTiles.reserve(4);

	int index = rand() % 7;
	for (int i = (index * 8); i < (index + 1) * 8; i += 2)
	{
		m_GameData.NextTiles.emplace_back(index, 5 + shapeDirs[i], 20 + shapeDirs[i + 1], color);
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
			m_GameLogicData.Points += 5;
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
	for (int i = 0; i < 4; i++)
	{
		newPositions[i].first = m_GameData.MovingTiles[i].X;
		newPositions[i].second = m_GameData.MovingTiles[i].Y;
	}
	GameTile::Rotate(newPositions, m_GameData.MovingTiles[0].rotState, m_GameData.MovingTiles[0].type);

	if (CheckRotated(newPositions))
	{
		for (int i = 0; i < 4; i++)
		{
			m_GameData.MovingTiles[i].X = newPositions[i].first;
			m_GameData.MovingTiles[i].Y = newPositions[i].second;
			m_GameData.MovingTiles[i].rotState = (m_GameData.MovingTiles[i].rotState + 1) % 4;
		}
	}
}

bool Game::CheckRotated(std::pair<int, float>* positions)
{
	bool correct = true;

	for (int i = 0; i < 4; i++)
	{
		positions[i].second = std::floor(positions[i].second);
		if (positions[i].first < 0 || positions[i].first > 9 ||
			positions[i].second < 0 || m_GameData.Tiles.find(positions[i]) != m_GameData.Tiles.end())
		{
			correct = false;
			break;
		}
	}

	return correct;
}

void Game::AddPointsToHighScores(int point)
{
	if (point <= m_GameLogicData.HighScores[9])
		return;

	bool found = false;
	int i = 9;
	while (i >= 0 && point > m_GameLogicData.HighScores[i])
	{
		i--;
	}

	for (int j = 9; j > i + 1; j--)
	{
		m_GameLogicData.HighScores[j] = m_GameLogicData.HighScores[j - 1];
	}

	// Insert the new score
	m_GameLogicData.HighScores[i + 1] = point;

	std::ofstream outFile("res/highscores.txt");

	if (!outFile)
		return;

	for (int i = 0; i < 10; i++)
	{
		outFile << m_GameLogicData.HighScores[i] << "\n";
	}

	outFile.close();
}

void Game::InitHighScores()
{
	std::ifstream inFile("res/highscores.txt");

	if (!inFile)
		return;

	for (int i = 0; i < 10; i++)
	{
		inFile >> m_GameLogicData.HighScores[i];
	}
	inFile.close();
}