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