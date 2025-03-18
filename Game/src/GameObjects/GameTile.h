#pragma once
#include <cstdint>

#include <glm/glm.hpp>

struct GameTile
{
	GameTile()
	{
		X = 4;
		Y = 20;
	}

	GameTile(int32_t x, int32_t y, const glm::vec4& color)
		:X(x), Y(y), Color(color)
	{
	}

	float X, Y;
	glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};