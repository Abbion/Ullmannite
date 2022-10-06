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
		Camera();
		~Camera() {}

		void Render() override {}

		void SetProjectionType(ProjectionType type);

		void GetProjectionMatrix();
		void GetViewMatrix();

	private:

	};
}