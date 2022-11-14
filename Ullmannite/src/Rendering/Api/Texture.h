#pragma once
#include "Rendering/Api/GraphicsTypes.h"

namespace Ull
{
    enum class PixelDataFormat
    {
        R,
        RG,
        RGB,
        RGBA,

        R_I,
        RB_I,
        RGB_I
    };

    enum class InternalDataFormat
    {
        RGBA_32F,
        RGB_32F,

        R_8I,
        R_8UI,

        R_16UI,
        RG_16UI,
        RGB_16UI,
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

    enum class ReadWriteRights
    {
        READ,
        WRITE,
        READ_AND_WRITE
    };

    class Texture1D
    {
    public:
        virtual ~Texture1D() {}

        static Texture1D* Create();

        std::uint16_t GetSize() { return m_size; }

        virtual void SetData(std::uint16_t size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data) = 0;
        virtual void EnableMinMap() const = 0;
        virtual void SetWrap(WrapMode horizontalWrap) const = 0;
        virtual void SetBorderColor(glm::vec4 color) const = 0;
        virtual void SetSampling(Sampling magSampling, Sampling minSampling) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const = 0;
    protected:
        Texture1D() = default;

        std::uint16_t m_size{ 0 };
    };

    class Texture2D
    {
    public:
        virtual ~Texture2D() {}

        static Texture2D* Create();

        glm::uvec2 GetSize() { return m_size; }
        
        virtual void SetData(glm::uvec2 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data) = 0;
        virtual void EnableMinMap() const = 0;
        virtual void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap) const = 0;
        virtual void SetBorderColor(glm::vec4 color) const = 0;
        virtual void SetSampling(Sampling magSampling, Sampling minSampling) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const = 0;
    protected:
        Texture2D() = default;

        glm::uvec2 m_size{ 0, 0 };
    };

    class Texture3D
    {
    public:
        virtual ~Texture3D() {}

        static Texture3D* Create();

        glm::uvec3 GetSize() { return m_size; }

        virtual void SetData(glm::uvec3 size, InternalDataFormat internalDataFormat, PixelDataFormat pixelDataFormat, GraphicsDataType dataType, const void* data) = 0;
        virtual void EnableMinMap() const = 0;
        virtual void SetWrap(WrapMode horizontalWrap, WrapMode verticalWrap, WrapMode depthWrap) const = 0;
        virtual void SetBorderColor(glm::vec4 color) const = 0;
        virtual void SetSampling(Sampling magSampling, Sampling minSampling) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void BindImage(InternalDataFormat internalDataFormat, ReadWriteRights readWriteRights, std::uint8_t bindIndex) const = 0;
    protected:
        Texture3D() = default;
        
        glm::uvec3 m_size{ 0, 0, 0 };
    };
}