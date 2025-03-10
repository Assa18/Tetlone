#include "WindowsWindow.h"

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}

void WindowsWindow::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Tetlone", nullptr, nullptr);
	if (m_Window == nullptr)
	{
		std::cout << "ERROR: failed to crate glfw window!\n";
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(m_Window);

	glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

}

void WindowsWindow::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

bool WindowsWindow::IsClosed()
{
	return glfwWindowShouldClose(m_Window);
}

void WindowsWindow::SetClosed(bool value)
{
	glfwSetWindowShouldClose(m_Window, value);
}

void WindowsWindow::Resize(int32_t width, int32_t height)
{
	
}

void* WindowsWindow::GetNativeWindow()
{
	return m_Window;
}


WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}