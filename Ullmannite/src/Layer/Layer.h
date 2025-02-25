#pragma once
#include "UiElement/UiRenderArea.h"

namespace Ull
{
    class Layer : protected UiRenderArea
    {
    public:
        Layer(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const bool usesDepth);

        void HandleEvent(Event* event) override;

        UiRenderArea::Update;
        virtual void Render() override final;
        virtual void RenderLayerComponents() = 0;

        UiRenderArea::GetName;
        UiRenderArea::GetChildren;

    protected:
        virtual void CreateLayout() = 0;
        virtual void ResizeLayout() = 0;
        void CreateRenderArea(const glm::vec2 position, const glm::vec2 size);

    private:
        glm::mat4 m_viewMatrix;
    };
};