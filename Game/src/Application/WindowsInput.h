#pragma once

#include "Input.h"
#include "Game.h"

#include "WindowsKeyCodes.h"

static bool previousKeys[GLFW_KEY_LAST] = { false };

inline bool Input::IsKeyJustPressed(int keyCode)
{
	bool isPressed = Input::IsKeyPressed(keyCode);
	bool wasPressed = previousKeys[keyCode];

	previousKeys[keyCode] = isPressed;

	return isPressed && !wasPressed;
}

inline bool Input::IsKeyPressed(int keyCode)
{
	auto window = static_cast<GLFWwindow*>(Game::GetWindow()->GetNativeWindow());
	return glfwGetKey(window, keyCode) == GLFW_PRESS;
}