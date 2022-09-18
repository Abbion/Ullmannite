#pragma once
#include "Rendering/Api/Texture.h"

namespace Ull
{
    class Texture2DOpenGL : public Texture2D
    {
    public:
        Texture2DOpenGL();
        ~Texture2DOpenGL();

        unsigned int GetOpenGLTextureID() const { return m_textureID; }

        void SetData(glm::uvec2 size, ColorFormat InChannel, ColorFormat OutChannel, const uint8_t* data) override;
        void EnableMinMap(bool minMap) const override;
        void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const override;
        void SetBorderColor(glm::vec4 color) const override;
        void SetSampling(Sampling magSampling, Sampling minSampling) const override;

        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_textureID{ 0 };
    };
    
};