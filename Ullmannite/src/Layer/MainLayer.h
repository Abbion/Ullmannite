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
        MainLayer(const glm::uvec2 size);

        void SetWindow(const NotOwner<UllWindow>& window);

        virtual void RenderLayerComponents() override;

    private:
        virtual void CreateLayout() override;
        virtual void ResizeLayout() override;

        NotOwner<UllWindow> m_window{ nullptr };

        std::shared_ptr<UiTitleBar> m_titleBar;
        std::shared_ptr<UiMenuView> m_menuView;
        std::shared_ptr<UiView3D> m_3DView;
    };
    
}
