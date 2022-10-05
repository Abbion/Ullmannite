#pragma once
#include "Scene/Nodes/Node3D.h"
#include "Scene/Nodes/Renderable.h"
#include <glm/glm.hpp>

namespace Ull
{
	class Cube : public Node3D, Renderable
	{
	public:
		Cube();
		~Cube();

		glm::vec4 GetColor() { return m_color; }
		void SetColor(glm::vec4 color) { m_color = color; }

	private:
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}