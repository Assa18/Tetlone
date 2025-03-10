#include "Camera2D.h"

#include <Application/WindowsInput.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Camera2D::Camera2D()
	:Camera()
{
	m_AspectRatio = 1.0f;
}

Camera2D::Camera2D(const glm::mat4& projection, float aspectRatio)
	:Camera(projection), m_AspectRatio(aspectRatio)
{
	m_ZoomLevel = 1.0f;

	m_ProjMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void Camera2D::OnUpdate(float deltaTime)
{
	if (Input::IsKeyPressed(GAME_KEY_UP))
	{
		m_ZoomLevel += 0.01;
		m_ProjMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	if (Input::IsKeyPressed(GAME_KEY_DOWN))
	{
		m_ZoomLevel -= 0.01;
		m_ProjMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
}

void Camera2D::OnResize(float width, float height)
{
	m_AspectRatio = width / height;
	m_ProjMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

const glm::mat4& Camera2D::GetViewProjMatrix() const
{
	return m_ProjMatrix;
}