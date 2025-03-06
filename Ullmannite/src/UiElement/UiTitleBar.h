#pragma once
#include <memory>
#include "UiRenderArea.h"
#include "UiElement/Controls/AllControls.h"

namespace Ull
{
	class UiTitleBar : public UiRenderArea
	{
	public:
		enum class TitleBarFunctionality: uint8_t{
			MINIMIZE = 1 << 0,
			RESTORE = 1 << 1,
			CLOSE = 1 << 2,
			TITLE = 1 << 3
		};

	public:
		UiTitleBar(const std::string& name, const glm::uvec2 position, const glm::uvec2 size);
		virtual ~UiTitleBar() {}

		RectF GetGrabArea() const;

		void HandleEvent(Event* event) override;
		void ResizeControls();

		void SetTitleText(const std::wstring& titleText);
		void SetTitleBarFunctionality(const TitleBarFunctionality functionality, const State state);

		void SetMinimizeFunction(std::function<void()> function) { m_minimizeFunction = function; }
		void SetMaximizeFunction(std::function<void()> function) { m_maximizeFunction = function; }
		void SetRestoreFunction(std::function<void()> function) { m_restoreFunction = function; }
		void SetCloseFunction(std::function<void()> function) { m_closeFunction = function; }

	private:
		void CreateControls();

		std::shared_ptr<UiButton> m_minimizeButton;
		std::shared_ptr<UiToggle> m_restoreButton;
		std::shared_ptr<UiButton> m_closeButton;
		std::shared_ptr<UiText> m_titleText;

		std::function<void()> m_minimizeFunction;
		std::function<void()> m_maximizeFunction;
		std::function<void()> m_restoreFunction;
		std::function<void()> m_closeFunction;

		using Functionality = uint8_t;
		Functionality m_functionality{ 0u };
	};
}