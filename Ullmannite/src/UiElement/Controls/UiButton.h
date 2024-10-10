#pragma once
#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiText.h"

namespace Ull
{
    class UiButton : public UiBasicControl
    {
    public:
        UiButton(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiButton() {}

        UiText& GetTextControl() { return m_buttonText; }

        void SetOnClickFunction(std::function<void(UiButton& buttonElement)> onClick) { m_onClick = onClick; }

        void SetSize(const glm::uvec2 size);

        void HandleEvent(Event* event);
        void Update() override;
        void Render();

    private:
        UiText m_buttonText;

        bool m_pressed{ false };
        std::function<void(UiButton& buttonElement)> m_onClick;
    };
}