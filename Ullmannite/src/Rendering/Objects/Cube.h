#pragma once
#include "Scene/Nodes/Node3D.h"
#include "Scene/Scene.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include <memory>
#include <glm/glm.hpp>

namespace Ull
{
	class Cube : public Node3D, virtual public Drawable
	{
	public:
		Cube(const std::string& name, const Scene* scene);
		~Cube() {}

		void CreateResources();
		void Update() override;
		void Render() override;

		glm::vec3 GetColor() { return m_color; }
		void SetColor(glm::vec3 color) { m_color = color; }

	private:

		glm::vec3 m_color{ 1.0f, 0.4f, 1.0f };
	};
}