#include "SoundEngine.h"

irrklang::ISoundEngine* SoundEngine::m_Engine = nullptr;

void SoundEngine::Initialize()
{
	m_Engine = irrklang::createIrrKlangDevice();
}

void SoundEngine::PlaySound(const char* path, bool repeat)
{
	m_Engine->play2D(path, repeat);
}