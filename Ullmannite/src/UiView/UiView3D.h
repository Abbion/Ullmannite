#pragma once
#include "UiElement/UiArea.h"
#include "Scene/Scene.h"
#include "Window/UllWindow.h"

namespace Ull
{
	class UiView3D : public UiArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);
		
		void SetWindow(const std::shared_ptr<UllWindow>& window);

		void Init();

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

	private:
		Scene m_scene;
		std::shared_ptr<UllWindow> m_window;
	};
}
