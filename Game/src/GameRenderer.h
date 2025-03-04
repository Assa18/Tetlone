#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLObjects/Shader.h"

struct GameSate
{

};

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void Initialize();
	void Draw();

private:
	Shader m_Shader;
};