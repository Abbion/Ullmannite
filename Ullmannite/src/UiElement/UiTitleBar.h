#pragma once
#include "UiArea.h"
#include "Window/Window.h"

namespace Ull
{
	class UiTitleBar : public UiArea
	{
	public:
		UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size);
		~UiTitleBar() {}

		bool WasClosePressed();
		bool WasMinimizePressed();
		bool WasMaximizeRestorePressed();
		bool IsOnDragArea() { return m_onDragArea; }

		void CreateResources() override;

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

		void RenderUI();
	private:

		bool m_onDragArea{ false };
		bool m_closePressed{ false };
		bool m_minimizePressed{ false };
		bool m_maximizeRestorPressed{ false };
	};
}