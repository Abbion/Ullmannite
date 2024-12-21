#pragma once
#include "UiElement/UiElement.h"

namespace Ull
{
    enum class UiControlType {
        UiButton,
        UiText,
        UiToggle,
        UiSpace
    };

    enum class UiControlFunctionality : uint64_t {
        Hover = 1 << 1
    };

    class UiBasicControl : public UiElement
    {
    public:
        virtual ~UiBasicControl() {}

        void SetFunctionality(UiControlFunctionality functionality, State state);
        bool IsFunctionalitySet(UiControlFunctionality functionality);

        void SetBackgroundColor(const glm::vec4& backgroundColor) { m_backgroundColor = backgroundColor; }
        void SetHoverColor(const glm::vec4& hoverColor) { m_hoverColor = hoverColor; }

        void CreateResources() override;
        void HandleEvent(Event* event) override;
        virtual void Update() override;
        virtual void Render() override;

    protected:
        UiBasicControl(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const UiControlType type);

        const UiControlType m_uiControlType;

        void CheckHover();

        glm::vec4 m_backgroundColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec4 m_hoverColor{ 1.0f, 1.0f, 1.0f, 1.0f };

        bool m_hover{ false };
        glm::mat4 m_perspective;

    private:
        inline void UpdatePerspective();

        using Functionality = uint64_t;
        Functionality m_functionality{ ULLONG_MAX };
    };
}