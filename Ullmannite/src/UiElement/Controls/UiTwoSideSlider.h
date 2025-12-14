#pragma once

#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiSpace.h"
#include "UiElement/Controls/UiText.h"

namespace Ull
{
    enum class UiTwoSideSliderHandle
    {
        NONE,
        MIN,
        MAX,
        MAIN
    };

    class UiTwoSideSlider : public UiBasicControl
    {
    public:
        UiTwoSideSlider(const std::string& name, const glm::vec2 position, const glm::vec2 size, const float limitMinValue, const float limitMaxValue);

        float GetMinSliderValue() const;
        float GetMaxSliderValue() const;
        std::shared_ptr<UiSpace> GetMainSliderHandle() { return m_mainSlider; }

        void SetSize(const glm::vec2 size);
        void SetMinValue(const float min);
        void SetMaxValue(const float max);

        void HandleEvent(Event* event) override;
        void Update() override;

    private:
        void CreateControls();
        void UpdateSliderValues();

        std::shared_ptr<UiSpace> m_minSlider;
        std::shared_ptr<UiSpace> m_maxSlider;
        std::shared_ptr<UiSpace> m_mainSlider;
        std::shared_ptr<UiText> m_minSliderValueText;
        std::shared_ptr<UiText> m_maxSliderValueText;
        glm::vec2 m_minDragBounderies;
        glm::vec2 m_maxDragBounderies;

        float m_limitMinValue{ 0.0f };
        float m_limitMaxValue{ 1.0f };

        float m_minValue{ 0.0f };
        float m_maxValue{ 1.0f };

        glm::ivec2 m_grabPoint{ 0.0f, 0.0f };
        UiTwoSideSliderHandle m_activeHandle{ UiTwoSideSliderHandle::NONE };
        glm::vec4 m_mainSliderBaseColor;
        bool m_mainSliderBaseColorSampled{ false };
    };
}
