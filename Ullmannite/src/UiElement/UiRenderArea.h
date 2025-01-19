#pragma once
#include "UiElement/UiElement.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include "Rendering/Api/Buffer.h"

namespace Ull
{
    class UiRenderArea : public UiElement
    {
    public:
        UiRenderArea(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const bool usesDepth);
        virtual ~UiRenderArea();

        void CreateResources() override;
        void SetBackgroundColor(const glm::vec4& color);
        void BindTargetTexture();

        void HandleEvent(Event* event) override;
        void Update() override;
        void Render() override;

    protected:
        void RenderBackground();
        void CheckMouseInArea();
        void CreateFrameBuffer();

        FrameBuffer* m_frameBuffer{ nullptr };
        bool m_areaUpdated{ false };
        bool m_inArea{ false };
        glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };

        bool m_usesDepth{ false };
    };
}