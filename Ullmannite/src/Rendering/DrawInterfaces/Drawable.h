#pragma once
#include "Ullpch.h"
#include "Rendering/Api/Buffer.h"
#include "Rendering/Api/Shader.h"
#include "Renderable.h"

namespace Ull
{
    class Drawable : public Renderable
    {
    public:
        virtual ~Drawable();

    protected:
        Drawable() {};
        Drawable(Drawable&& source);

        VertexBuffer* m_vertexBuffer{ nullptr };
        IndexBuffer* m_indexBuffer{ nullptr };
        VertexLayout* m_layout{ nullptr };
        std::shared_ptr<Shader> m_shader{ nullptr };
    };
}