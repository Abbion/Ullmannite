#include "Ullpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include <algorithm>

using namespace Ull;

namespace
{
	constexpr float cameraRotationSensitivityScale = 15.0f;
	constexpr float cameraPanSensitivityScale = 0.2f;
}

Camera::Camera(const std::string& name, const Scene* scene, glm::uvec2 renderAreaSize) :
	 Node3D(name, scene),
	 m_renderAreaSize(renderAreaSize),
	 m_aspectRatio((float)renderAreaSize.x / (float)renderAreaSize.y)
{
	SetVisibility(false);
}

void Camera::HandleEvent(Event* event)
{
	if(event->GetType() == EventType::KeyDown)
	{
		if(static_cast<KeyDownEvent*>(event)->GetVal() == Keyboard::Key::F && Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::L_CONTROL))
		{
			m_cameraType = m_cameraType == CameraType::LOCKED ? CameraType::FREE : CameraType::LOCKED; 
		}
	}
	else if(event->GetType() == EventType::MouseMove)
	{
		if(m_cameraType == CameraType::FREE && m_window->IsCursorLocked())
		{
			CalculatePitchAndYaw();
			UpdateVectors();
		}
		else if( m_cameraType == CameraType::LOCKED)
		{
			if(Mouse::GetInstance().IsButtonPressed(Mouse::Button::LEFT))
			{
				CalculatePitchAndYaw(cameraRotationSensitivityScale);
				UpdateVectors();
				m_position = m_target - glm::vec3(m_front.x * m_distanceToTarget, m_front.y * m_distanceToTarget, m_front.z * m_distanceToTarget);
			}
			else if(Mouse::GetInstance().IsButtonPressed(Mouse::Button::MIDDLE))
			{
				auto mouseDelta = Mouse::GetInstance().GetMousePositionDelta();
				auto xSens = -mouseDelta.x * m_sensitivity * cameraPanSensitivityScale;
				auto ySens = mouseDelta.y * m_sensitivity * cameraPanSensitivityScale;

				m_target += glm::vec3(xSens * m_right.x, xSens * m_right.y, xSens * m_right.z);
				m_target += glm::vec3(ySens * m_up.x, ySens * m_up.y, ySens * m_up.z);
			}
		}
	}
	else if(event->GetType() == EventType::MouseScroll)
	{
		m_fov += static_cast<MouseScrollEvent*>(event)->GetVal();
		m_fov = std::clamp(m_fov, m_minFov, m_maxFov);
		CalculateProjectionMatrix();
	}

	Node::HandleEvent(event);
}

void Camera::Update()
{
	if(m_cameraType == CameraType::LOCKED && Mouse::GetInstance().IsButtonPressed(Mouse::Button::LEFT))
	{

	}

	else if(m_cameraType == CameraType::FREE && m_window->IsCursorLocked())
	{
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::W))
		{
			m_position += m_front * m_speed;
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::S))
		{
			m_position -= m_front * m_speed;
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::A))
		{
			m_position -= m_right * m_speed;
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::D))
		{
			m_position += m_right * m_speed;
		}

		UpdateVectors();
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
	if(m_cameraType == CameraType::LOCKED)
	{
		m_view = glm::lookAt(m_position + m_target, m_target, m_up);
	}
	else
	{
		m_view = glm::lookAt(m_position, m_position + m_front, m_up);
	}
}

void Camera::CalculateProjectionMatrix()
{
	if (m_projectionType == ProjectionType::PERSPECTIVE)
	{
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 10.f);
	}
	else
	{
		m_projection = glm::ortho(0.0f, (float)m_renderAreaSize.x, 0.0f, (float)m_renderAreaSize.y);
	}
}

void Camera::UpdateVectors()
{
	glm::vec3 front;
	front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	front.y = std::sin(glm::radians(m_pitch));
	front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUP));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::CalculatePitchAndYaw(float sensitivityScale)
{
	auto mouseDelta = Mouse::GetInstance().GetMousePositionDelta();

	m_yaw += mouseDelta.x * m_sensitivity * sensitivityScale;
	m_pitch += -mouseDelta.y * m_sensitivity * sensitivityScale;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;

    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
}