#pragma once
#include <cstdint>

class Window
{
public:
	Window()
		:m_Width(1280), m_Height(720)
	{
	}


	Window(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
	}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual bool IsClosed() = 0;
	virtual void SetClosed(bool value) = 0;
	virtual void Resize(int32_t width, int32_t height) = 0;

	virtual void* GetNativeWindow() = 0;
protected:
	uint32_t m_Width, m_Height;
};