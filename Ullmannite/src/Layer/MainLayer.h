#pragma once
#include "Layer.h"
#include "Window/UllWindow.h"
#include <glm/glm.hpp>
#include "Utilities/PointerHelper.h"

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
    };
    
}
