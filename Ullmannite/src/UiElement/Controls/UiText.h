#pragma once

#include "UiElement/Controls/UiBasicControl.h"
#include "Resources/Fonts/FontManager.h"

namespace Ull
{
    class UiText : public UiBasicControl
    {
    public:
        enum class VerticalAlignment : uint8_t
        {
            TOP,
            CENTER,
            BOTTOM
        };

        enum class HorizontalAlignment : uint8_t
        {
            LEFT,
            CENTER,
            RIGHT
        };

    public:
        UiText(const std::string name, const glm::uvec2 position, const glm::uvec2 size, const std::wstring text);
        ~UiText() {}

        const std::wstring& GetString() const   { return m_text; }
        glm::vec4 GetColor() const              { return m_color; }
        unsigned int GetFontSize() const        { return m_fontSize; }
        float GetSpaceing() const               { return m_spaceing; }
        float GetLeading() const                { return m_leading; }
        float GetEdgeSmoothing() const          { return m_smoothing; }
        float GetSampleThreshold() const        { return m_threshold; }
        glm::uvec2 GetTextSize();

        void SetSize(const glm::uvec2 size);

        void SetString(std::wstring& text);
        void SetFont(const FontTag font);
        void SetColor(const glm::vec4 color);
        void SetFontSize(const unsigned size);
        void SetSpaceing(const float spaceing);
        void SetLeading(const float leading);
        void SetAlignment(const HorizontalAlignment hAlignment, const VerticalAlignment vAlignment);
        void SetEdgeSmoothing(const float smoothing);
        void SetSampleThreshold(const float threshold);
        void SetSmoothingExceptance(std::initializer_list<wchar_t> exceptance);

        void CreateResources() override;
        void Update() override;
        void Render() override;

    private:
        void UpdateDisplayTextSize();
        void RealignText();

        std::wstring        m_text;
        glm::vec3           m_cursorPos{ 0.0f, 0.0f, 0.0f };
        glm::vec2           m_displayTextSize{ 0.0f, 0.0f };

        FontTag             m_fontTag{ FontTag::UI_FONT };
        glm::vec4           m_color{ 1.0f, 1.0f, 1.0f, 1.0f };

        unsigned int        m_fontSize{ 20u };
        float               m_spaceing{ 10.0f };
        float               m_leading{ 1.25f };
        HorizontalAlignment m_horizontalAlignment{ HorizontalAlignment::LEFT };
        VerticalAlignment   m_verticalAlignment{ VerticalAlignment::TOP };

        float                   m_smoothing{ 5.0f };
        float                   m_threshold{ 50.0f };
        std::vector<wchar_t>    m_smoothingExceptance{};

        glm::mat4x4         m_textAlignmentlMatrix;
        
        bool                m_updateDrawData{ true };
    };
}