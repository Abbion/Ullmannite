#pragma once
#include "Ullpch.h"

namespace Ull
{
    enum class ColorFormat
    {
        R,
        RG,
        RGB,
        RGBA
    };

    enum class WrapMode
    {
        REPAT,
        MIRROR_REPEAT,
        CLAMP,
        BORDER
    };

    enum class Sampling
    {
        NEAREST,
        LINEAR
    };

    class Texture2D
    {
    public:
        virtual ~Texture2D() {}

        static Texture2D* Create();

        glm::uvec2 GetSize() { return m_size; }
        
        virtual void SetData(glm::uvec2 size, ColorFormat InChannel, ColorFormat OutChannel, const uint8_t* data) = 0;
        virtual void EnableMinMap(bool minMap) const = 0;
        virtual void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const = 0;
        virtual void SetBorderColor(glm::vec4 color) const = 0;
        virtual void SetSampling(Sampling magSampling, Sampling minSampling) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
    protected:
        Texture2D() = default;

        glm::uvec2 m_size{0, 0};
    };
}