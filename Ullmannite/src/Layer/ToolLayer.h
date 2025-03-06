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
    
        virtual void RenderLayerComponents() override;
    
    private:
        glm::mat4 m_viewMatrix;
    
    private:
        virtual void CreateLayout() override;
        virtual void ResizeLayout() override;
    
        std::shared_ptr<UiPickerView> m_pickerView;
    };
}