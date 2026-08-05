#pragma once
#include "UiElement.h"
#include <glm/glm.hpp>
#include "Scene/Scene.h"

namespace Ull
{
	class UiSceneView : public UiElement
	{
	public:
		UiSceneView(const std::string& name);

		Scene& GetScene() { return m_scene; }
		void CreateResources();

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;
	
	private:
		Scene m_scene;
	};
}