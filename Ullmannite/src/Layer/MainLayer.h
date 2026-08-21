#pragma once
#include "Layer.h"
#include "Window/UllWindow.h"
#include <glm/glm.hpp>
#include "Utilities/PointerHelper.h"
#include "UiElement/UiTitleBar.h"
#include "UiView/UiMenuView.h"
#include "UiView/UiView3D.h"

namespace Ull
{
    class MainLayer : public Layer
    {
    public:
        MainLayer(const glm::uvec2 size, const NotOwner<LayerManager>& layerManager);

        virtual void RenderLayerComponents() override;

    private:
        virtual void CreateLayout() override;
        virtual void ResizeLayout() override;

        std::shared_ptr<UiTitleBar> m_titleBar;
        std::shared_ptr<UiRenderArea> m_titleBarView;

        std::shared_ptr<UiMenuView> m_menuView;
        std::shared_ptr<UiView3D> m_3DView;
    };
    
}
