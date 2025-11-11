#pragma once

#include "UiElement/Controls/UiBasicControl.h"
#include "UiElement/Controls/UiSpace.h"

namespace Ull
{
    class UiMarker : public UiBasicControl
    {
    public:
        UiMarker(const std::string name, const glm::vec2 position, const glm::vec2 size);

        float GetPositionRatio() const;
        glm::vec4 GetColor() const { return m_markerColor->GetBackgroundColor(); }

        void CreateResources() override;
        void SetMarkerColor(const glm::vec4 color);
        void SetDragBounderies(const float left, const float right);

        void HandleEvent(Event* event) override;

    private:
        void CreateControls();
        bool m_grabbed{ false };
        glm::ivec2 m_grabPoint{ 0.0f, 0.0f };

        std::shared_ptr<UiSpace> m_markerColor;
        std::shared_ptr<UiSpace> m_markerColorOutline;
        glm::vec2 m_dragBounderies;
    };
}