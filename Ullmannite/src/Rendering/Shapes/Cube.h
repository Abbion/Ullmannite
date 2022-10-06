#pragma once
#include "Scene/Nodes/Node3D.h"
#include "Scene/Nodes/RenderableNode.h"
#include "Scene/Scene.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include <memory>
#include <glm/glm.hpp>

namespace Ull
{
	class Cube : public RenderableNode, virtual public Drawable
	{
	public:
		Cube();
		~Cube() {}

		void CreateResources();
		void Render() override;

		glm::vec4 GetColor() { return m_color; }
		void SetColor(glm::vec4 color) { m_color = color; }

	private:

		glm::vec4 m_color{ 1.0f, 0.4f, 1.0f, 1.0f };
	};
}