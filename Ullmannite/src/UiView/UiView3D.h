#pragma once
#include "UiElement/UiRenderArea.h"
#include "Scene/Scene.h"
#include "Window/UllWindow.h"
#include "Utilities/PointerHelper.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <memory>
#include "UiElement/Controls/UiText.h"

namespace Ull
{
	class UiView3D : public UiRenderArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);
		void SetTransferFunction(const NotOwner<TransferFunctionRenderer>& transferFunction);
		
		void SetWindow(const NotOwner<UllWindow>& window);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void Init();

		Scene m_scene;
		NotOwner<UllWindow> m_window{ nullptr };
		NotOwner<TransferFunctionRenderer> m_transferFunction{ nullptr };

		std::shared_ptr<UiText> m_titleText;
	};
}
