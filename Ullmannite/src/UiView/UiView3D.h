#pragma once
#include "UiElement/UiArea.h"
#include "Scene/Scene.h"

namespace Ull
{
	class UiView3D : public UiArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);
		
		void Init();

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

	private:
		Scene m_scene;
	};
}
