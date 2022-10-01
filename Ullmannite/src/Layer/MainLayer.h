#pragma once
#include "Layer.h"
#include "Window/Window.h"
#include <glm/glm.hpp>

namespace Ull
{
    class MainLayer : public Layer
    {
    public:
        MainLayer(glm::uvec2 size);

        void SetWindow(std::shared_ptr<UllWindow> window) { m_window = window; }

        void Update();
        void Render();

        void HandleEvent(Event* event) override;

        void Init();

    private:
        void CreateLayout();
        void Resize(const glm::uvec2& size);

        bool m_checkClickInteractions{ false };

        glm::mat4 m_viewMatrix;
        std::shared_ptr<UllWindow> m_window;
    };
    
}
