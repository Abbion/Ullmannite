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

    constexpr GLenum ConverterChannel(ColorFormat channel)
    {
        switch (channel)
        {
        case ColorFormat::R:
            return GL_RED;
            break;

        case ColorFormat::RG:
            return GL_RG;
            break;

        case ColorFormat::RGB:
            return GL_RGB;
            break;

        case ColorFormat::RGBA:
            return GL_RGBA;
            break;

        default:
            ULOGE("Can't conver this channel");
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
}