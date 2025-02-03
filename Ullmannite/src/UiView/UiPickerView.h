#pragma once
#include "UiElement/Controls/AllControls.h"
#include "UiElement/UiRenderArea.h"

namespace Ull
{
    class UiPickerView : public UiRenderArea
    {
    public:
        UiPickerView(const std::string& name, const glm::uvec2 position);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

    private:
		void CreateControls();

        std::shared_ptr<UiLinearColorGradient> m_linearGradient;
        std::shared_ptr<UiSpace> m_linearGradientPick;

        std::shared_ptr<UiRectGradient> m_colorGradientRect;
        std::shared_ptr<UiSpace> m_horizontalPick;
        std::shared_ptr<UiSpace> m_verticalPick;

        std::shared_ptr<UiSpace> m_selectedColorRect;
    };
}