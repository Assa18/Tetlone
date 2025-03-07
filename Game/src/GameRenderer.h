#pragma once

#include <glad/glad.h>

#include "OpenGLObjects/Shader.h"
#include "OpenGlObjects/Camera2D.h"

struct GameData
{
	Camera2D Camera;

	void Update()
	{
		Camera.OnUpdate(0.01f);
	}
};

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialize();
	void Draw();

	inline GameData* GetGameData() { return &m_GameData; }
private:
	Shader m_Shader;
	GameData m_GameData;
};