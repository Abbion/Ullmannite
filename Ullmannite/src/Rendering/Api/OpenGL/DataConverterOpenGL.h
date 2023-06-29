#pragma once
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "Rendering/Api/GraphicsTypes.h"
#include "Rendering/Api/Buffer.h"

namespace Ull
{
    constexpr GLenum ConvertUsageType(const GraphicsBufferType& type)
    {
        switch (type)
        {
        case GraphicsBufferType::STATIC_DRAW:
            return GL_STATIC_DRAW;
            break;

        case GraphicsBufferType::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
            break;
        case GraphicsBufferType::STREAM_DRAW:
            return GL_STREAM_DRAW;
            break;

        default:
            ULOGE("Cannot convert this type");
            return 0;
            break;
        }
    }

    constexpr GLenum ConvetDataType(const GraphicsDataType& type)
    {
        switch (type)
        {
        case GraphicsDataType::FLOAT:
            return GL_FLOAT;
            break;

        case GraphicsDataType::DOUBLE:
            return GL_DOUBLE;
            break;

        case GraphicsDataType::INT:
            return GL_INT;
            break;

        case GraphicsDataType::UINT:
            return GL_UNSIGNED_INT;
            break;

        case GraphicsDataType::HALF:
            return GL_HALF_FLOAT;
            break;

        case GraphicsDataType::SHORT:
            return GL_SHORT;
            break;

        case GraphicsDataType::USHORT:
            return GL_UNSIGNED_SHORT;
            break;

        case GraphicsDataType::BYTE:
            return GL_BYTE;
            break;

        case GraphicsDataType::UBYTE:
            return GL_UNSIGNED_BYTE;
            break;

        default:
            ULOGE("Cannot convert this type");
            return 0;
            break;
        }
    }

    constexpr uint16_t SizeOfDataType(const GraphicsDataType& type)
    {
        switch (type)
        {
        case GraphicsDataType::INT:
        case GraphicsDataType::UINT:
        case GraphicsDataType::FLOAT:
            return sizeof(int32_t);
            break;

        case GraphicsDataType::DOUBLE:
            return sizeof(int64_t);
            break;

        case GraphicsDataType::HALF:
            return sizeof(int16_t);
            break;

        case GraphicsDataType::BYTE:
        case GraphicsDataType::UBYTE:
            return sizeof(int8_t);
            break;

        default:
            ULOGE("Cannot convert this type");
            return 0;
            break;
        }
    }

    constexpr GLenum ConvertRenderFufferFormat(RenderBuffer::Format format)
    {
        switch (format)
        {
        case RenderBuffer::Format::DEPTH:
            return GL_DEPTH_COMPONENT24;
            break;

        case RenderBuffer::Format::DEPTH_STENCIL:
            return GL_DEPTH24_STENCIL8;
            break;

        default:
            ULOGE("Cannot convert this type");
            return 0;
            break;
        }
    }

    constexpr GLenum ConverterPixelFormat(PixelDataFormat channel)
    {
        switch (channel)
        {
        case PixelDataFormat::R:
            return GL_RED;
            break;

        case PixelDataFormat::RG:
            return GL_RG;
            break;

        case PixelDataFormat::RGB:
            return GL_RGB;
            break;

        case PixelDataFormat::RGBA:
            return GL_RGBA;
            break;

        case PixelDataFormat::R_I:
            return GL_RED_INTEGER;
            break;

        case PixelDataFormat::RG_I:
            return GL_RG_INTEGER;

        case PixelDataFormat::RGB_I:
            return GL_RGB_INTEGER;

        default:
            ULOGE("Can't conver this pixel format");
            return 0;
            break;
        }
    }

    constexpr GLenum ConverterInternalFormat(InternalDataFormat channel)
    {
        switch (channel)
        {
        case InternalDataFormat::RGBA_32F:
            return GL_RGBA32F;
            break;

        case InternalDataFormat::RGB_32F:
            return GL_RGB32F;
            break;

        case InternalDataFormat::R_8F:
            return GL_R8;
            break;

        case InternalDataFormat::R_8I:
            return GL_R8I;

        case InternalDataFormat::R_8UI:
            return GL_R8UI;

        case InternalDataFormat::R_16UI:
            return GL_R16UI;
            break;

        case InternalDataFormat::RG_16UI:
            return GL_RG16UI;
            break;

        case InternalDataFormat::RGB_16UI:
            return GL_RGB16UI;
            break;

        case InternalDataFormat::R_32I:
            return GL_R32I;
        break;

        case InternalDataFormat::R_32UI:
            return GL_R32UI;
        break;

        default:
            ULOGE("Can't conver this internal format");
            return 0;
            break;
        }
    }

    constexpr GLenum ConverterReadWriteRights(ReadWriteRights readWriteRights)
    {
        switch (readWriteRights)
        {
        case ReadWriteRights::READ:
            return GL_READ_ONLY;
            break;

        case ReadWriteRights::WRITE:
            return GL_WRITE_ONLY;
            break;

        case ReadWriteRights::READ_AND_WRITE:
            return GL_READ_WRITE;
            break;

        default:
            ULOGE("Can't conver this read/write rights");
            return 0;
            break;
        }
    }

    constexpr GLenum ConverterWrap(WrapMode wrap)
    {
        switch (wrap)
        {
        case WrapMode::BORDER:
            return GL_CLAMP_TO_BORDER;
            break;

        case WrapMode::CLAMP:
            return GL_CLAMP_TO_EDGE;
            break;

        case WrapMode::MIRROR_REPEAT:
            return GL_MIRRORED_REPEAT;
            break;

        case WrapMode::REPAT:
            return GL_REPEAT;
            break;

        default:
            ULOGE("Can't conver this wrap type");
            return 0;
            break;
        }
    }

    constexpr GLenum SamplerConverter(Sampling sampling)
    {
        switch (sampling)
        {
        case Sampling::LINEAR:
            return GL_LINEAR;
            break;

        case Sampling::NEAREST:
            return GL_NEAREST;
            break;

        default:
            ULOGE("Can't conver this sampling type");
            return 0;
            break;
        }
    }

    constexpr GLenum ConvertPrimitive(GraphicsRenderPrimitives primitive)
    {
        switch (primitive)
        {
        case GraphicsRenderPrimitives::TRIANGLE:
            return GL_TRIANGLES;
            break;

        case GraphicsRenderPrimitives::LINE:
            return GL_LINES;
            break;
        case GraphicsRenderPrimitives::POINT:
            return GL_POINTS;
            break;

        default:
            ULOGE("Cannot convert this primitive");
            return 0;
            break;
        }
    }

    constexpr GLenum ConvertFillType(GraphicsRenderPrimitives primitive)
    {
        switch (primitive)
        {
        case GraphicsRenderPrimitives::TRIANGLE:
            return GL_FILL;
            break;

        case GraphicsRenderPrimitives::LINE:
            return GL_LINE;
            break;
        case GraphicsRenderPrimitives::POINT:
            return GL_POINT;
            break;

        default:
            ULOGE("Cannot convert this primitive");
            return 0;
            break;
        }
    }
}