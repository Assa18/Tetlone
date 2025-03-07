#pragma once

#include "Input.h"
#include "Game.h"

#include "WindowsKeyCodes.h"

bool Input::IsKeyPressed(int keyCode)
{
	auto window = static_cast<GLFWwindow*>(Game::GetWindow()->GetNativeWindow());
	return glfwGetKey(window, keyCode) == GLFW_PRESS;
}