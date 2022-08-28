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

    class Texture2D
    {
    public:
        NON_COPYABLE(Texture2D)

        virtual ~Texture2D() {}

        static Texture2D* Create(unsigned int windth, unsigned int height, ColorChannels channel, const uint8_t* data);
        
        virtual void Bind();
        virtual void Unbind();
        
    protected:
        Texture2D() = default;
    };
}