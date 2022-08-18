#include "Ullpch.h"
#include "Drawable.h"

using namespace Ull;

DrawableInterface::~DrawableInterface()
{
    if(m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    
    if(m_vertexBuffer != nullptr)
        delete m_indexBuffer;

    //TODO: check if shader should be deleted
    //TODO: check if layout should be deleted
}