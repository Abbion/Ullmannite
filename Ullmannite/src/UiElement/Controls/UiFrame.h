#pragma once

#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiFrame : public UiBasicControl
    {
    public:
        UiFrame(const std::string name, const glm::vec2 position, const glm::vec2 size);

        float GetThickness() const { return m_thickness; }

        void SetThickness(const float thicness) { m_thickness = thicness; }
        virtual void CreateResources();

    private:
        float m_thickness{ 1.0f };
    };
}