#pragma once
#include "UiElement/UiRenderArea.h"
#include "UiElement/UiSceneView.h"
#include "Utilities/PointerHelper.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <memory>

namespace Ull
{
	class UiView3D : public UiRenderArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void Init();

		std::shared_ptr<UiSceneView> m_sceneview{ nullptr };
		std::unique_ptr<TransferFunctionRenderer> m_transferFunction{ nullptr };
	};
}
