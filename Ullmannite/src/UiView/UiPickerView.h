#pragma once
#include "UiElement/Controls/AllControls.h"
#include "UiElement/UiRenderArea.h"
#include "UiElement/UiTitleBar.h"

namespace Ull
{
    class UiPickerView : public UiRenderArea
    {
    public:
        UiPickerView(const std::string& name, const glm::uvec2 position, const glm::vec4 startingColor, const onColorChangeCallback& callback);

        bool IsReadyToRelease() const { return m_readyToRelease; }

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

    private:
        UiRenderArea::SetSize;

		void CreateControls();
        void UpdateLinearPickerPosition(const glm::vec2 cursorPosition);
        void UpdateGradientRectPickerPosition(const glm::vec2 cursorPosition);
        void UpdateSelectors();
        void UpdateValueFields();

        const glm::vec4 m_startingColor;
        glm::vec4 m_currentColor;
        onColorChangeCallback m_onColorChangeCallback;

        std::shared_ptr<UiFrame> m_frame;
        std::shared_ptr<UiTitleBar> m_titleBar;

        bool m_isSubWindowGrabbed{ false };
        bool m_readyToRelease{ false };
        glm::ivec2 m_grabStartPosition{ 0, 0 };

        bool m_isLinearGradientInteracting{ false };
        bool m_isGradientRectInteracting{ false };
        std::shared_ptr<UiLinearColorGradient> m_linearGradient;
        std::shared_ptr<UiSpace> m_linearGradientPick;

        std::shared_ptr<UiRectGradient> m_colorGradientRect;
        std::shared_ptr<UiSpace> m_horizontalPick;
        std::shared_ptr<UiSpace> m_verticalPick;

        std::shared_ptr<UiSpace> m_selectedColorRect;
        std::shared_ptr<UiSpace> m_selectedColorRectOutline;

        std::shared_ptr<UiButton> m_confirmButton;
        std::shared_ptr<UiButton> m_restoreButton;
        std::shared_ptr<UiButton> m_cancelButton;

        std::shared_ptr<UiText> m_redValueFieldLabel;
        std::shared_ptr<UiText> m_greenValueFieldLabel;
        std::shared_ptr<UiText> m_blueValueFieldLabel;

        std::shared_ptr<UiNumberField> m_redValueNumberField;
        std::shared_ptr<UiNumberField> m_greenValueNumberField;
        std::shared_ptr<UiNumberField> m_blueValueNumberField;
    };
}