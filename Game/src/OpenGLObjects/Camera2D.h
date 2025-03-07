#pragma once

#include "Camera.h"

class Camera2D : public Camera
{
public:
	Camera2D();
	Camera2D(const glm::mat4& projection, float aspectRatio);

	void OnUpdate(float deltaTime) override;
	void OnResize(float width, float height) override;

	const glm::mat4& GetViewProjMatrix() override;
private:
	float m_AspectRatio;
	float m_ZoomLevel;
};

