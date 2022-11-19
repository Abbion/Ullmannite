#pragma once
#include "UiElement/UiArea.h"
#include "Scene/Scene.h"
#include "Window/UllWindow.h"
#include "Utilities/PointerHelper.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <memory>

namespace Ull
{
	class UiView3D : public UiArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);
		
		void SetWindow(const NotOwner<UllWindow>& window);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void Init();

		Scene m_scene;
		NotOwner<UllWindow> m_window{ nullptr };

		std::shared_ptr<TransferFunctionRenderer> m_transferFunciton;
	};
}
