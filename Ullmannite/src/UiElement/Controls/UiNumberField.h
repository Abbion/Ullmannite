#pragma once
#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiText.h"
#include "UiElement/Controls/UiSpace.h"

namespace Ull
{
    class UiNumberField : public UiBasicControl
    {
    public:
        UiNumberField(const std::string& name, const glm::vec2 position, const glm::vec2 size, const float startNumber, const bool useDecimals = true);

        float GetNumber() const;
        std::shared_ptr<UiText> GetTextControl() { return m_numberText; };

        void SetSize(const glm::vec2 size);
        void SetNumber(float number);
        void SetMinNumber(const float minNumber);
        void SetMaxNumber(const float maxNumber);

        void SetOnNumberConfirmedFunction(std::function<void(const float)> onNumberConfirmedFunction);

        void HandleEvent(Event* event);
        void Update() override;
        void Render();

    private:
        void UpdateCursorPosition();
        void ResetCursorBlink();
        bool CreateHightlightBox();
        void ClearFiled();
        void ConfirmValue();

        std::shared_ptr<UiText>     m_numberText;
        std::shared_ptr<UiSpace>    m_cursor;
        std::shared_ptr<UiSpace>    m_selectedHighlight;

        const bool m_useDecimals;
        float m_currentNumber;
        float m_minNumber{ -255.0f };
        float m_maxNumber{ 255.0f };

        bool m_fullSelect{ false };
        bool m_isEdited{ false };
        bool m_isEmpty{ false };
        unsigned m_cursorPosition{ 0u };

        double m_lsatBlinkTime = 0.0f;

        std::optional<std::function<void(const float)>> m_onNumberConfirmedFunction;
    };
}