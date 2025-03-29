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
		rotState = 0;
	}

	GameTile(int type, int32_t x, int32_t y, const glm::vec4& color)
		:type(type), X(x), Y(y), Color(color)
	{
		rotState = 0;
	}

	int type;
	float X, Y;
	int rotState;
	glm::vec4 Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	static void Rotate(std::pair<int, float> positions[4], int rotState, int type)
	{
		int index = 0;
		rotState = (rotState + 1) % 4;
		int initX = positions[0].first;
		int initY = std::round(positions[0].second);
		for (int i = 0; i < 4; i++)
		{
			positions[i].first = initX + transformations[type * 32 + rotState * 8 + index];
			index++;
			positions[i].second = initY + transformations[type * 32 + rotState * 8 + index];
			index++;
		}
	}

private:
	inline static int transformations[] = {
		0, 0, 0, 1, 0, 2, 0, 3, // I
		0, 0, 1, 0, 2, 0, 3, 0,
		0, 0, 0, 1, 0, 2, 0, 3,
		0, 0, 1, 0, 2, 0, 3, 0,

		0, 0, 1, 0, 1, 1, 1, 2, // J
		0, 0, 0, 1, 1, 0, 2, 0,
		0, 0, 0, 1, 0, 2, 1, 2,
		0, 1, 1, 1, 2, 1, 2, 0,

		0, 0, 0, 1, 0, 2, 1, 0, // L
		0, 0, 0, 1, 1, 1, 2, 1,
		0, 2, 1, 2, 1, 1, 1, 0,
		0, 0, 1, 0, 2, 0, 2, 1,

		0, 0, 1, 0, 0, 1, 1, 1, // O
		0, 0, 1, 0, 0, 1, 1, 1,
		0, 0, 1, 0, 0, 1, 1, 1,
		0, 0, 1, 0, 0, 1, 1, 1,

		0, 0, 1, 0, 1, 1, 2, 1, // S
		0, 1, 0, 2, 1, 1, 1, 0,
		0, 0, 1, 0, 1, 1, 2, 1,
		0, 1, 0, 2, 1, 1, 1, 0,

		0, 1, 1, 1, 1, 0, 2, 1, // T
		0, 0, 0, 1, 0, 2, 1, 1,
		0, 0, 1, 0, 2, 0, 1, 1,
		0, 1, 1, 0, 1, 1, 1, 2,

		0, 1, 1, 1, 1, 0, 2, 0, // Z
		0, 0, 0, 1, 1, 1, 1, 2,
		0, 1, 1, 1, 1, 0, 2, 0,
		0, 0, 0, 1, 1, 1, 1, 2,
	};
};