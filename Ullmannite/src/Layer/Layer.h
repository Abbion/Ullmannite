#pragma once
#include "UiElement/UiRenderArea.h"

namespace Ull
{
    namespace LayerNames
    {
        constexpr auto mainLayer = "MainLayer";
        constexpr auto toolLayer = "ToolLayer";
    }

    class LayerManager;
    class MainLayer;
    class ToolLayer;

    class Layer : protected UiRenderArea
    {
    public:
        Layer(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const bool usesDepth, const NotOwner<LayerManager>& layerManager);

        void HandleEvent(Event* event) override;

        UiRenderArea::Update;
        virtual void Render() override final;
        virtual void RenderLayerComponents() = 0;

        UiRenderArea::GetName;
        UiRenderArea::GetChildren;
        UiRenderArea::RemoveChildNode;

    protected:
        virtual void CreateLayout() = 0;
        virtual void ResizeLayout() = 0;
        void CreateRenderArea(const glm::vec2 position, const glm::vec2 size);
        NotOwner<LayerManager> m_layerManager;

    private:
        glm::mat4 m_viewMatrix;

        friend MainLayer;
        friend ToolLayer;
    };
};