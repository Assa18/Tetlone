#pragma once

#include "irrKlang/irrKlang.h"

class SoundEngine
{
public:
	static void Initialize();
	static void PlaySound(const char* path, bool repeat);
private:
	static irrklang::ISoundEngine* m_Engine;
};