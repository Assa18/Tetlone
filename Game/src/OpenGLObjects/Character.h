#pragma once

#include "glm/glm.hpp"

struct Character
{
	glm::vec2 Size;
	glm::vec2 Bearing;
	uint32_t Advance;
	glm::vec2 TexPos;
	glm::vec2 TexSize;
};