#pragma once

namespace Ull
{
    enum class GraphicsBufferType
    {
        STATIC_DRAW,
        DYNAMIC_DRAW,
        STREAM_DRAW
    };

    enum class GraphicsDataType
    {
        HALF,
        FLOAT,
        DOUBLE,
        INT,
        UINT,
        BYTE,
        UBYTE,
    };

    enum class GraphicsShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        COMPUTE,
        PROGRAM
    };

    enum class GraphicsRenderPrimitives
    {
        TRIANGLE,
        LINE,
        POINT
    };
}