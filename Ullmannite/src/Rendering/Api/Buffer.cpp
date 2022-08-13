#include "Ullpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "OpenGL/BufferOpenGL.h"

using namespace Ull;

VertexBuffer* VertexBuffer::Creeate(int size, float* data, GraphicsBufferType type)
{
    switch (Renderer::GetInstance()->GetApi())
    {
        case Renderer::API::OPEN_GL:
            return new VertexBufferOpenGL(size, data, type);
        break;
    }

    //TODO error message
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(int size, int* data, GraphicsBufferType type)
{
    switch (Renderer::GetInstance()->GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new IndexBufferOpenGL(size, data, type);
    break;
    }

    //TODO error message
    return nullptr;
}

VertexLayout* VertexLayout::Create(std::initializer_list<VertexLayoutElement> initList)
{
    switch (Renderer::GetInstance()->GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new VertexLayoutOpenGL(initList);
    break;
    }

    //TODO error message
    return nullptr;
}