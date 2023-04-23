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
        MainLayer(glm::uvec2 size);

        void SetWindow(const NotOwner<UllWindow>& window);

        void HandleEvent(Event* event) override;
        void Update();
        void Render();

    private:
        glm::mat4 m_viewMatrix;

    private:
        void CreateLayout();
        void Resize(const glm::uvec2& size);

        std::shared_ptr<UiTitleBar> m_titleBar;
        std::shared_ptr<UiMenuView> m_menuView;
        std::shared_ptr<UiView3D> m_3DView;
    };
    
}
