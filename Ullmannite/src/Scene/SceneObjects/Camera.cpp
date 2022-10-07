#include "Ullpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Input/Keyboard.h"

using namespace Ull;

#define UP_DIR glm::vec3(0.0f, 1.0f, 0.0f)

Camera::Camera(const std::string& name, const Scene* scene, glm::uvec2 renderAreaSize) :
	 Node3D(name, scene),
	 m_renderAreaSize(renderAreaSize),
	 m_aspectRatio((float)renderAreaSize.x / (float)renderAreaSize.y)
{
	SetVisibility(false);
}

void Camera::Update()
{
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::A))
	{
		m_position.x += 0.005f;
	}
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::D))
	{
		m_position.x -= 0.005f;
	}
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::W))
	{
		m_position.y += 0.005f;
	}
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::S))
	{
		m_position.y -= 0.005f;
	}
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::UP))
	{
		m_position.z -= 0.005f;
	}
	if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::DOWN))
	{
		m_position.z += 0.005f;
	}


	CalculateViewMatrix();
}

void Camera::SetRenderAreaSize(glm::uvec2 renderAreaSize)
{
	m_renderAreaSize = renderAreaSize;
	m_aspectRatio = (float)m_renderAreaSize.x / (float)m_renderAreaSize.x;
}

void Camera::CalculateViewMatrix()
{
	m_view = glm::lookAt(m_position, m_target, UP_DIR);
}

void Camera::CalculateProjectionMatrix()
{
	if (m_type == ProjectionType::PERSPECTIVE)
	{
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 10.f);
	}
	else
	{
		m_projection = glm::ortho(0.0f, (float)m_renderAreaSize.x, 0.0f, (float)m_renderAreaSize.y);
	}
}