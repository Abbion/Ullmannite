#pragma once
#include "Ullpch.h"
#include "Rendering/Api/Buffer.h"
#include "Rendering/Api/Shader.h"

namespace Ull
{
    class DrawableInterface
    {
    public:
        DrawableInterface() {};
        virtual ~DrawableInterface();

        inline bool IsVisible() { return m_visible; }
        void SetVisible(bool visible) { m_visible = visible; }

        virtual void Render() = 0;
        
    protected:
        VertexBuffer* m_vertexBuffer{ nullptr };
        IndexBuffer* m_indexBuffer{ nullptr };
        VertexLayout* m_layout{ nullptr };
        Shader* m_shader{ nullptr };

        bool m_visible{ true };
    };
}