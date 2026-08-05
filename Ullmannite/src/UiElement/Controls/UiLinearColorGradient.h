#pragma once
#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiLinearColorGradient : public UiBasicControl
    {
    public:
        enum class GradientDirection
        {
            HORIZONTAL,
            VERTICAL
        };

    public:
        UiLinearColorGradient(const std::string name, const glm::vec2 position, const glm::vec2 size, const GradientDirection gradientDirection);
        void SetInteractive(const bool state) { m_isInteractive = state; }
        void AddColor(const GradientColorData colorData);
        void ClearColorData();

        const std::vector<GradientColorData>& GetGradientColors() const { return m_gradientColors; }

        glm::vec4 GetColorFromLastInteraction() const { return m_lastInteractionColor; }
        glm::vec4 GetColorForRatio(const float ratio);

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        virtual void Update() override;
        virtual void Render() override;

    private:
        void InteractWithMouse();

        std::vector<GradientColorData> m_gradientColors;
        const GradientDirection m_gradientDirection;

        bool m_isInteractive{ false };
        glm::vec4 m_lastInteractionColor{ 0.0f, 0.0f, 0.0f, 0.0f };
    };
}