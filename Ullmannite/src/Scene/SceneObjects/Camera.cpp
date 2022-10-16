#include "Ullpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Scene/Scene.h"
#include <algorithm>

using namespace Ull;

namespace
{
	constexpr float cameraRotationSensitivityScale = 15.0f;
	constexpr float cameraPanSensitivityScale = 0.1f;
}

Camera::Camera(const std::string& name, NotOwner<Scene> scene, glm::uvec2 renderAreaSize) :
	 Node3D(name, scene),
	 m_renderAreaSize(renderAreaSize),
	 m_aspectRatio((float)renderAreaSize.x / (float)renderAreaSize.y)
{
	SetVisibility(false);
}

void Camera::HandleEvent(Event* event)
{
	if (event->IsHandeled())
		return;

	if(event->GetType() == EventType::KeyDown)
	{
		auto key = static_cast<KeyDownEvent*>(event)->GetVal();

		if(Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::L_CONTROL))
		{
			if(key == Keyboard::Key::F)
			{
				if (m_cameraType == CameraType::FREE)
				{
					auto distanceFromFront = m_front * m_distanceToTarget;
					m_target = m_position + distanceFromFront;
					m_orbitPosition = -distanceFromFront;
				}

				m_cameraType = m_cameraType == CameraType::LOCKED ? CameraType::FREE : CameraType::LOCKED;

				m_scene->SetUpdated(true);
			}
			else if(key == Keyboard::Key::R)
			{
				ResetCamera();
				m_scene->SetUpdated(true);
			}
		}
	}
	else if(event->GetType() == EventType::MouseMove)
	{
		if(m_cameraType == CameraType::FREE && m_window->IsCursorLocked())
		{
			CalculatePitchAndYaw();
			UpdateVectors();

			m_scene->SetUpdated(true);
		}
		else if( m_cameraType == CameraType::LOCKED)
		{
			if(Mouse::GetInstance().IsButtonPressed(Mouse::Button::RIGHT))
			{
				CalculatePitchAndYaw(cameraRotationSensitivityScale);
				UpdateVectors();

				m_orbitPosition = m_front * -m_distanceToTarget;

				m_scene->SetUpdated(true);
			}
			else if(Mouse::GetInstance().IsButtonPressed(Mouse::Button::MIDDLE))
			{
				auto mouseDelta = Mouse::GetInstance().GetMousePositionDelta();
				auto xSens = mouseDelta.x * m_sensitivity * cameraPanSensitivityScale;
				auto ySens = mouseDelta.y * m_sensitivity * cameraPanSensitivityScale;

				m_target += m_right * -xSens; //X is inverted
				m_target += m_up * ySens;

				m_scene->SetUpdated(true);
			}
		}
	}
	else if(event->GetType() == EventType::MouseScroll)
	{
		m_fov += static_cast<MouseScrollEvent*>(event)->GetVal();
		m_fov = std::clamp(m_fov, m_minFov, m_maxFov);
		CalculateProjectionMatrix();

		m_scene->SetUpdated(true);
	}

	Node::HandleEvent(event);
}

void Camera::Update()
{
	 if(m_cameraType == CameraType::FREE && m_window->IsCursorLocked())
	{
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::W))
		{
			m_position += m_front * m_speed;
			m_scene->SetUpdated(true);
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::S))
		{
			m_position -= m_front * m_speed;
			m_scene->SetUpdated(true);
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::A))
		{
			m_position -= m_right * m_speed;
			m_scene->SetUpdated(true);
		}
		if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::D))
		{
			m_position += m_right * m_speed;
			m_scene->SetUpdated(true);
		}
	}

	CalculateViewMatrix();
}

void Camera::SetRenderAreaSize(glm::uvec2 renderAreaSize)
{
	m_renderAreaSize = renderAreaSize;
	m_aspectRatio = (float)m_renderAreaSize.x / (float)m_renderAreaSize.y;
	CalculateProjectionMatrix();
}

void Camera::CalculateViewMatrix()
{
	if(m_cameraType == CameraType::LOCKED)
	{
		m_position = m_orbitPosition + m_target;
		m_view = glm::lookAt(m_position, m_target, m_up);
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
		m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 10.0f);
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
	m_pitch -= mouseDelta.y * m_sensitivity * sensitivityScale;

    if (m_pitch > 89.9f)
        m_pitch = 89.9f;

    if (m_pitch < -89.9f)
        m_pitch = -89.9f;

	if(m_yaw > 360)
		m_yaw = m_yaw - 360;
	else if(m_yaw < -360)
		m_yaw = m_yaw + 360;
}

void Camera::ResetCamera()
{
	m_distanceToTarget = 3.0f;
	m_position = glm::vec3(0.0f, 0.0f, m_distanceToTarget);
	m_orbitPosition = glm::vec3(0.0f, 0.0f, m_distanceToTarget);
	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(1.0f, 0.0f, 0.0f);

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_fov = 45.f;

	UpdateVectors();
}