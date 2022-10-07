#pragma once
#include "Scene/Nodes/Node3D.h"

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

	public:
		Camera(const std::string& name, const Scene* scene, glm::uvec2 renderAreaSuze);
		~Camera() {}

		glm::mat4 GetProjectionMatrix() const { return m_projection; }
		glm::mat4 GetViewMatrix() const { return m_view; }

		void Update() override;

		void SetRenderAreaSize(glm::uvec2 renderAreaSize);
		void SetProjectionType(ProjectionType type) { m_type = type; }
		void SetTarget(glm::vec3 target) { m_target = target; }
		
		void CalculateProjectionMatrix();
		void CalculateViewMatrix();
		
		void Render() override {}

	private:
		ProjectionType m_type{ ProjectionType::PERSPECTIVE };

		glm::vec3 m_target{ 0.f, 0.0f, 0.0f, };

		glm::mat4 m_view{ 1.0f };
		glm::mat4 m_projection{ 1.0f };

		float m_fov{ 45 };
		float m_aspectRatio{ 16.0f / 9.0f };
		glm::uvec2 m_renderAreaSize{ 1, 1 };
	};
}