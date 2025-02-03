#pragma once
#include "Layer.h"
#include "UiView/UiPickerView.h"
#include <glm/glm.hpp>

namespace Ull
{
    class ToolLayer : public Layer
    {
    public:
        ToolLayer(const glm::uvec2 size);

        void HandleEvent(Event* event) override;
        void Update();
        void Render();

    private:
        glm::mat4 m_viewMatrix;

    private:
        void CreateLayout();
        void Resize(const glm::uvec2 size);

        std::shared_ptr<UiPickerView> m_pickerView;
    };
}