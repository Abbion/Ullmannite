#pragma once
#include "UiElement/UiRenderArea.h"
#include "Scene/Scene.h"
#include "Window/UllWindow.h"
#include "Utilities/PointerHelper.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <memory>

namespace Ull
{
	class UiView3D : public UiRenderArea
	{
	public:
		UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size);
		void SetTransferFunction(const NotOwner<TransferFunctionRenderer>& transferFunction);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

	private:
		void Init();

		Scene m_scene;
		NotOwner<TransferFunctionRenderer> m_transferFunction{ nullptr };
	};
}
