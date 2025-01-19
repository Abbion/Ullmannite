#pragma once
#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiText.h"

namespace Ull
{
    class UiColorGradient : public UiBasicControl
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
        UiColorGradient(std::string name, glm::vec2 position, glm::vec2 size, GradientDirection gradientDirection);
        void AddColor(const GradientColorData colorData);
        void ClearColorData();

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        virtual void Update() override {}
        virtual void Render() override;

    private:
        bool m_updateVertexData{ true };
        std::vector<GradientColorData> m_gradientColors;
        const GradientDirection m_gradientDirection;
    };
}