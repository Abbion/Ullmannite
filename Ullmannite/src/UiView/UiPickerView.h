#pragma once
#include "UiElement/Controls/AllControls.h"
#include "UiElement/UiRenderArea.h"
#include "UiElement/UiTitleBar.h"

namespace Ull
{
    class UiPickerView : public UiRenderArea
    {
    public:
        UiPickerView(const std::string& name, const glm::uvec2 position);

        bool IsReadyToRelease() const { return m_readyToRelease; }

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

        virtual void OnPositionChange() override;

    private:
        UiRenderArea::SetSize;

		void CreateControls();

        std::shared_ptr<UiFrame> m_frame;
        std::shared_ptr<UiTitleBar> m_titleBar;

        RectF m_grabArea;
        bool m_isGrabbed{ false };
        bool m_readyToRelease{ false };
        glm::ivec2 m_grabStartPosition{ 0, 0 };

        std::shared_ptr<UiLinearColorGradient> m_linearGradient;
        std::shared_ptr<UiSpace> m_linearGradientPick;

        std::shared_ptr<UiRectGradient> m_colorGradientRect;
        std::shared_ptr<UiSpace> m_horizontalPick;
        std::shared_ptr<UiSpace> m_verticalPick;

        std::shared_ptr<UiSpace> m_selectedColorRect;
    };
}