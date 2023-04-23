#pragma once
#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiToggle : public UiBasicControl
    {
    public:
        UiToggle(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiToggle() {}

        bool IsEnabled() const { return m_state; }

        void SetEnabled(const bool state) { m_state = state; }
        void SetOnEnabledFunction(std::function<void(UiToggle& toggleElement)> onEnabled) { m_onEnabled = onEnabled; }
        void SetOnDisambledFunction(std::function<void(UiToggle& toggleElement)> onDisabled) { m_onDisabled = onDisabled; }

        void HandleEvent(Event* event);
        void Update() override;
        void Render();

    private:
        bool m_pressed{ false };
        bool m_state{ false };

        std::function<void(UiToggle& toggleElement)> m_onEnabled;
        std::function<void(UiToggle& toggleElement)> m_onDisabled;
    };
}