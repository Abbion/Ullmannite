#pragma once
#include <memory>
#include "UiRenderArea.h"
#include "Window/UllWindow.h"
#include "UiElement/Controls/AllControls.h"

namespace Ull
{
	class UiTitleBar : public UiRenderArea
	{
	public:
		UiTitleBar(const std::string& name, const glm::uvec2 position, const glm::uvec2 size);
		virtual ~UiTitleBar() {}

		void HandleEvent(Event* event) override;

		void SetWindow(const NotOwner<UllWindow>& window) { m_window = window; }
		void ResizeControls();

	private:
		void CreateControls();

		NotOwner<UllWindow> m_window{ nullptr };

		std::shared_ptr<UiButton> m_closeButton;
		std::shared_ptr<UiButton> m_minimizeButton;
		std::shared_ptr<UiToggle> m_restoreButton;
		std::shared_ptr<UiText> m_titleText;
	};
}