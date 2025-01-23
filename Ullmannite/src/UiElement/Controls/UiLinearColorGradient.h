#pragma once
#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiText.h"

namespace Ull
{
    class UiLinearColorGradient : public UiBasicControl
    {
    public:
        struct GradientColorData
        {
            float position;
            glm::vec4 color;
        };

        enum class GradientDirection
        {
            HORIZONTAL,
            VERTICAL
        };

    public:
        UiLinearColorGradient(std::string name, glm::vec2 position, glm::vec2 size, GradientDirection gradientDirection);
        void SetInteractive(bool state) { m_isInteractive = state; }
        void AddColor(const GradientColorData colorData);
        void ClearColorData();

        glm::vec4 GetColorFromLastInteraction() const { return m_lastInteractionColor; }

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        virtual void Update() override;
        virtual void Render() override;

    private:
        void InteractWithMouse();
        glm::vec4 GetColorForRatio(const float ratio);

        bool m_updateVertexData{ true };
        std::vector<GradientColorData> m_gradientColors;
        const GradientDirection m_gradientDirection;

        bool m_isInteractive{ false };
        glm::vec4 m_lastInteractionColor{ 0.0f, 0.0f, 0.0f, 0.0f };
    };
}