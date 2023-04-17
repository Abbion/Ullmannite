#pragma once
#include "UiElement/UiElement.h"

namespace Ull
{
    class UiBasicControl : public UiElement
    {
    public:
        UiBasicControl(std::string name, glm::uvec2 position, glm::uvec2 size);
        virtual ~UiBasicControl() {}

        void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }
        void SetHoverColor(const glm::vec4& hoverColor) { m_hoverColor = hoverColor; }

        void SetParent(NotOwner<UiElement> parent);

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        void Update() override;
        void Render() override;

    protected:
        void CheckHover();

        glm::vec4 m_backgroundColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec4 m_hoverColor{ 1.0f, 1.0f, 1.0f, 1.0f };

        bool m_hover{ false };
        glm::mat4 m_perspective;

    private:
        inline void UpdatePerspective();
    };
}