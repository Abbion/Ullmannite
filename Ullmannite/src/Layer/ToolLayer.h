#pragma once
#include "Layer.h"
#include "UiView/UiPickerView.h"
#include "Window/UllWindow.h""
#include <glm/glm.hpp>

namespace Ull
{
    class ToolLayer : public Layer
    {
    public:
        ToolLayer(const glm::uvec2 size, const NotOwner<LayerManager>& layerManager);
    
        virtual void Update() override;

        void SetWindow(const NotOwner<UllWindow>& window);
        virtual void RenderLayerComponents() override;

    private:
        glm::mat4 m_viewMatrix;
    
    private:
        void RegisterToolCreationEvents();

        virtual void CreateLayout() override;
        virtual void ResizeLayout() override;
    
        std::shared_ptr<UiPickerView> m_pickerView;
        NotOwner<UllWindow> m_window{ nullptr };
    };
}