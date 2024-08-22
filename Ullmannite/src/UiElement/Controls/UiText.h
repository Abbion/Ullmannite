#pragma once

#include "UiElement/Controls/UiBasicControl.h"
#include "Resources/Fonts/FontManager.h"

namespace Ull
{
    class UiText : public UiBasicControl
    {
    public:
        UiText(const std::string name, const glm::uvec2 position, const glm::uvec2 size, const std::wstring text);
        ~UiText() {}

        void SetString(std::wstring& text);
        void SetFont();
        void SetAlignment();
        void SetBold();
        void SetNormal();
        void SetItalic();

        void HandleEvent(Event* event) override {};
        void CreateResources() override;
        void Update() override;
        void Render() override;

    private:
        std::wstring m_text;
        unsigned int m_fontSize{ 30 };
    };
}