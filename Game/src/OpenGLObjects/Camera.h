#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera()
	{
		m_ProjMatrix = glm::mat4(1.0f);
	}

	Camera(const glm::mat4& projection)
	{
		m_ProjMatrix = projection;
	}


	virtual void OnUpdate(float deltaTime) = 0;

	virtual void OnResize(float width, float height) = 0;

	virtual const glm::mat4& GetViewProjMatrix() const = 0;
protected:
	glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
};

