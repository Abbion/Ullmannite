#pragma once
#include "UiArea.h"
#include "Window/Window.h"
#include <memory>

namespace Ull
{
	class UiTitleBar : public UiArea
	{
	public:
		UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size);
		~UiTitleBar() {}

		void SetWindow(const std::shared_ptr<UllWindow>& window) { m_window = window; }

		bool IsOnDragArea() { return m_onDragArea; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

		void RenderUI();
	private:
		std::shared_ptr<UllWindow> m_window;

		bool m_onDragArea{ false };
		bool m_closePressed{ false };
		bool m_minimizePressed{ false };
		bool m_maximizeRestorPressed{ false };
	};
}