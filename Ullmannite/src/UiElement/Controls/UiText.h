#pragma once

#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiText : public UiBasicControl
    {
    public:
        UiText(std::string name, glm::uvec2 position, glm::uvec2 size, std::wstring text);
        ~UiText() {}

        void SetString(std::wstring& text);
        void SetFont();
        void SetAlignment();
        void SetBold();
        void SetNormal();
        void SetItalic();

        void HandleEvent(Event* event) {};
        void Update() override {};
        void Render() {};

    private:
        std::wstring m_text;
    };
}