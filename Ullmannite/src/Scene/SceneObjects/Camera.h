#pragma once
#include "Scene/Nodes/Node3D.h"
#include "Window/UllWindow.h"

#include <memory.h>

namespace Ull
{
	class Camera : public Node3D
	{
	public:
		enum class ProjectionType
		{
			ORTOGRAPHICS,
			PERSPECTIVE
		};

		enum class CameraType
		{
			LOCKED,
			FREE
		};

	public:
		Camera(const std::string& name, const Scene* scene, glm::uvec2 renderAreaSuze);
		~Camera() {}

		void SetWindow(const std::shared_ptr<UllWindow>& window) { m_window = window; }
		void SetDistanceToTarget(float distance) { m_distanceToTarget = distance; }

		glm::mat4 GetProjectionMatrix() const { return m_projection; }
		glm::mat4 GetViewMatrix() const { return m_view; }

		void HandleEvent(Event* event) override;
		void Update() override;

		void SetSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
		void SetFreeModeSpeed(float speed) { m_speed = speed; }

		void SetRenderAreaSize(glm::uvec2 renderAreaSize);
		void SetProjectionType(ProjectionType type) { m_projectionType = type; }
		void SetCameraType(CameraType type) { m_cameraType = type; }
		void SetTarget(glm::vec3 target) { m_target = target; }
		
		void CalculateProjectionMatrix();
		void CalculateViewMatrix();
		
		void Render() override {}

	private:
		void UpdateVectors();
		void CalculatePitchAndYaw(float sensitivityScale = 1.0f);
		void ResetCamera();

		ProjectionType m_projectionType{ ProjectionType::PERSPECTIVE };
		CameraType m_cameraType{ CameraType::LOCKED };

		glm::vec3 m_target{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
		float m_distanceToTarget{ 3.0f };
		glm::vec3 m_orbitPosition{ 0.0f, 0.0f, 3.0f };

		const glm::vec3 m_worldUP{ 0.0f, 1.0f, 0.0f };

		glm::mat4 m_view{ 1.0f };
		glm::mat4 m_projection{ 1.0f };

		float m_aspectRatio{ 16.0f / 9.0f };
		glm::uvec2 m_renderAreaSize{ 1, 1 };

		float m_yaw{ -90.0f };
		float m_pitch{ 0.0f };
		float m_fov{ 45.0f };
		float m_sensitivity{ 0.05f };
		float m_speed{ 0.05f };

		float m_polar{0.0f};
		float m_azimuth{90.0f};

		const float m_maxFov{ 75.0f };
		const float m_minFov{ 10.0f };

		std::shared_ptr<UllWindow> m_window;
	};
}