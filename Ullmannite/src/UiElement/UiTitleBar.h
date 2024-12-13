#pragma once
#include "UiRenderArea.h"
#include "Window/UllWindow.h"
#include <memory>
#include "Utilities/PointerHelper.h"
#include "Controls/UiButton.h"
#include "Controls/UiToggle.h"
#include "Controls/UiText.h"

namespace Ull
{
	class UiTitleBar : public UiRenderArea
	{
	public:
		UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size);
		virtual ~UiTitleBar() {}

		void SetWindow(const NotOwner<UllWindow>& window) { m_window = window; }

		bool IsOnDragArea() { return m_onDragArea; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;
	private:
		void CreateControls();
		void ResizeControls();

		NotOwner<UllWindow> m_window{ nullptr };

		bool m_onDragArea{ false };

		std::shared_ptr<UiButton> m_closeButton;
		std::shared_ptr<UiButton> m_minimizeButton;
		std::shared_ptr<UiToggle> m_restoreButton;
		std::shared_ptr<UiText> m_titleText;
	};
}