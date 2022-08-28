#pragma once
#include "UiElement/UiElement.h"
#include "Rendering/Drawable/Drawable.h"

namespace Ull
{
    class UiArea : public UiElement, public DrawableInterface
    {
    public:
        UiArea(std::string name, glm::uvec2 position, glm::uvec2 size);
        ~UiArea() {}

        void CreateResources() override;
        void SetColor(const glm::vec4& color);
        
        void HandleEvent(Event* event) override;

        void Update() override;
        void Render() override;
    private:
        glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
    };
}