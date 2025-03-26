#pragma once
#include <cstdint>

#include <glm/glm.hpp>

struct GameTile
{
	GameTile()
	{
		X = 4;
		Y = 20;
		type = 0;
	}

	GameTile(int type, int32_t x, int32_t y, const glm::vec4& color)
		:type(type), X(x), Y(y), Color(color)
	{
	}

	int type;
	float X, Y;
	glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};