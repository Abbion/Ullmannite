#pragma once
#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiButton : public UiBasicControl
    {
    public:
        UiButton(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiButton() {}


        void SetOnClickFunction(std::function<void(UiButton& buttonElement)> onClick)      { m_onClick = onClick; }

        void HandleEvent(Event* event);
        void Update() override;
        void Render();

    private:
        bool m_pressed{ false };
        std::function<void(UiButton& buttonElement)> m_onClick;
    };
}