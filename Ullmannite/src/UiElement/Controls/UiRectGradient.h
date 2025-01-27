#pragma once
#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiRectGradient : public UiBasicControl
    {
    public:
        struct GradientColorData
        {
            unsigned index;
            glm::vec3 color;
        };

    public:
        UiRectGradient(const std::string name, const glm::vec2 position, const glm::vec2 size);

        void SetInteractive(const bool state) { m_isInteractive = state; }

        void SetColorData(std::initializer_list<GradientColorData> colors);
        void SetColorData(GradientColorData colorData);

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        virtual void Update() override;
        virtual void Render() override;

    private:
        void InteractWithMouse();
        glm::vec4 GetColorForRatio(const float ratio);

        bool m_updateVertexData{ true };
        std::array<glm::vec3, 4> m_gradientColors;

        bool m_isInteractive{ false };
        glm::vec4 m_lastInteractionColor{ 0.0f, 0.0f, 0.0f, 0.0f };
    };
}