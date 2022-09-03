#pragma once
#include "Ullpch.h"

namespace Ull
{
    enum class ColorChannels
    {
        R,
        G,
        B,
        RG,
        RB,
        GB,
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
        NON_COPYABLE(Texture2D)

        virtual ~Texture2D() {}

        static Texture2D* Create();

        glm::uvec2 GetSize() { return m_size; }
        ColorChannels GetChannels() { return m_channels; }
        
        virtual void SetData(unsigned int width, unsigned int height, ColorChannels InChannel, ColorChannels OutChannel, const uint8_t* data) const = 0;
        virtual void EnableMinMap(bool minMap) const = 0;
        virtual void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const = 0;
        virtual void SetBorderColor(glm::vec4 color) const = 0;
        virtual void SetSampling(Sampling magSampling, Sampling minSampling) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
    protected:
        Texture2D() = default;

    private:
        glm::uvec2 m_size;
        ColorChannels m_channels;
    };
}