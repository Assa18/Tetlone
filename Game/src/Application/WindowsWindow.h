#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

class WindowsWindow : public Window
{
public:
	WindowsWindow()
		:Window(), m_Window(nullptr)
	{
	}

	WindowsWindow(uint32_t width, uint32_t height)
		:Window(width, height)
	{
		Initialize();
	}

	~WindowsWindow();

	void Initialize() override;
	void Update() override;
	bool IsClosed() override;
	void SetClosed(bool value) override;
	void Resize(int32_t width, int32_t height) override;
	float GetTime() override
	{
		return glfwGetTime();
	}

	void* GetNativeWindow() override;
private:
	GLFWwindow* m_Window;
};

