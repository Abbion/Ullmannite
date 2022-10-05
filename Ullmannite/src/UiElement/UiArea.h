#pragma once
#include "UiElement/UiElement.h"
#include "Rendering/Drawable/Drawable.h"
#include "Rendering/Api/Buffer.h"

namespace Ull
{
    class UiArea : public UiElement
    {
    public:
        UiArea(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiArea();

        void CreateResources() override;
        void SetBackgroundColor(const glm::vec4& color);
        void BindTargetTexture();

        void HandleEvent(Event* event) override {};

        void Update() override {};
        void Render() override;

        void RenderBackground();

    protected:
        FrameBuffer* m_frameBuffer{ nullptr };
        bool m_areaUpdated{ false };
        glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    };
}