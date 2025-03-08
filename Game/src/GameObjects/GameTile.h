#pragma once
#include <cstdint>

struct GameTile
{
	GameTile()
	{
		IsMoving = true;
		x = 9;
		y = -1;
	}

	bool IsMoving;

	int32_t x, y;
};