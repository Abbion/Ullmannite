#pragma once
#include "Rendering/Api/Texture.h"
#include "DataConverterOpenGL.h"

namespace Ull
{
    class Texture2DOpenGL : public Texture2D
    {
    public:
        Texture2DOpenGL();
        ~Texture2DOpenGL();

        unsigned int GetOpenGLTextureID() const { return m_textureID; }

        void SetData(glm::uvec2 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data) override;
        void EnableMinMap() const override;
        void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const override;
        void SetBorderColor(glm::vec4 color) const override;
        void SetSampling(Sampling magSampling, Sampling minSampling) const override;

        void Bind() const override;
        void Unbind() const override;

        void BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const override;
    private:
        unsigned int m_textureID{ 0 };
    };
    
    class Texture3DOpenGL : public Texture3D
    {
    public:
        Texture3DOpenGL();
        ~Texture3DOpenGL();

        unsigned int GetOpenGLTextureID() const { return m_textureID; }

        void SetData(glm::uvec3 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data) override;
        void EnableMinMap() const override;
        void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap, WrapMode depthWrap) const override;
        void SetBorderColor(glm::vec4 color) const override;
        void SetSampling(Sampling magSampling, Sampling minSampling) const override;

        void Bind() const override;
        void Unbind() const override;

        void BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const override;
    private:
        unsigned int m_textureID{ 0 };
    };
};