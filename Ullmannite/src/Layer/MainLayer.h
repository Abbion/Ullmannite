#pragma once
#include "Layer.h"
#include <glm/glm.hpp>

namespace Ull
{
    class MainLayer : public Layer
    {
    public:
        MainLayer(glm::uvec2 size);

        void Update();
        void Render();

        void HandleEvent(Event* event) override;

        void Init();

    private:
        void CreateLayout();
        void Resize(const glm::uvec2& size);

        glm::mat4 m_viewMatrix;
    };
    
}
