#pragma once
#include "Ullpch.h"
#include "Rendering/Api/Buffer.h"
#include "Rendering/Api/Shader.h"

namespace Ull
{
    class DrawableInterface
    {
    public:
        //TODO: NON COPYABLE???
        virtual ~DrawableInterface();
        
    protected:
        DrawableInterface() {};

        VertexBuffer* m_vertexBuffer{ nullptr };
        IndexBuffer* m_indexBuffer{ nullptr };
        VertexLayout* m_layout{ nullptr };
        Shader* m_shader{ nullptr };
    };
}