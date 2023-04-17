#pragma once
#include "UiArea.h"
#include "Window/UllWindow.h"
#include <memory>
#include "Utilities/PointerHelper.h"
#include "Controls/UiButton.h"

namespace Ull
{
	class UiTitleBar : public UiArea
	{
	public:
		UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size);
		~UiTitleBar() {}

		void SetWindow(const NotOwner<UllWindow>& window) { m_window = window; }

		bool IsOnDragArea() { return m_onDragArea; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

		void RenderUI();
	private:
		void InitializeButtons();

		NotOwner<UllWindow> m_window{ nullptr };

		bool m_onDragArea{ false };
		bool m_closePressed{ false };
		bool m_minimizePressed{ false };
		bool m_maximizeRestorPressed{ false };

		UiButton m_closeButton;
		UiButton m_minimizeButton;
	};
}