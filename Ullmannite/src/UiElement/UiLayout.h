#pragma once
#include "UiElement/UiElement.h"
#include "Rendering/DrawingTools/FrameBufferRenderer.h"

namespace Ull
{
	class UiLayout : public UiElement
	{
	public:
		UiLayout(const std::string& name, const glm::uvec2 size);
		~UiLayout();

		void HandleEvent(Event* event) override;

		void Render() override;

	private:
		FrameBufferRenderer m_frameBufferRenderer;
		void CreateResources() {};
	};
}