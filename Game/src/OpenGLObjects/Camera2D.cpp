#include "Camera2D.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	//if (glfwGetKey(GLFW_KEY_UP))
}

void Camera2D::OnResize(float width, float height)
{
	m_AspectRatio = width / height;
	m_ProjMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

const glm::mat4& Camera2D::GetViewProjMatrix()
{
	return m_ProjMatrix;
}