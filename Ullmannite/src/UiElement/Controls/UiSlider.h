#pragma once
#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiText.h"
#include "UiElement/Controls/UiSpace.h"

namespace Ull
{
    class UiSlider : public UiBasicControl
    {
    public:
        UiSlider(std::string name, glm::vec2 position, glm::vec2 size);

        float GetValue() const { return m_sliderValue; }

        std::shared_ptr<UiText> GetTextControl() { return m_sliderText; }
        std::shared_ptr<UiSpace> GetSliderHandle() { return m_sliderHandle; }

        void SetValue(const float value);
        void SetMin(const float minValue);
        void SetMax(const float maxValue);
        
        void SetSize(const glm::vec2 size);

        void CreateResources() override;

        void HandleEvent(Event* event);
        void Update() override;
        void Render();

    private:
        void UpdateSliderValue();
        void UpdateSliderPosition();
        void UpdateText();

        std::shared_ptr<UiSpace> m_sliderHandle;
        std::shared_ptr<UiText> m_sliderText;

        glm::vec4 m_sliderBackgroundColorState;

        bool m_handleActive{ false };
        glm::vec2 m_handleGrabPosition{ 0.0f, 0.0f };
        float m_sliderValue{ 0.0f };

        float m_minValue{ 0.0f };
        float m_maxValue{ 1.0f };
    };
}