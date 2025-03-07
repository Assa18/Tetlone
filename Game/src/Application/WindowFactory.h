#pragma once

#include "Window.h"
#include "WindowsWindow.h"

class Windowfactory
{
public:
	static Window* GetWindow(uint32_t width, uint32_t height) { return new WindowsWindow(width, height); };
};