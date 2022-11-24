#include "Ullpch.h"
#include "Drawable.h"

using namespace Ull;

Drawable::~Drawable()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_vertexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;
}

Ull::Drawable::Drawable(Drawable&& source) :
    m_vertexBuffer{ source.m_vertexBuffer },
    m_indexBuffer{ source.m_indexBuffer },
    m_layout{ source.m_layout },
    m_shader{ source.m_shader }
{
    source.m_vertexBuffer = nullptr;
    source.m_indexBuffer = nullptr;
    source.m_layout = nullptr;
    source.m_shader = nullptr;
}
