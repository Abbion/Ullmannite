#include "Ullpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "OpenGL/BufferOpenGL.h"
#include "Logger/Logger.h"

using namespace Ull;

VertexBuffer* VertexBuffer::Create(int size, float* data, GraphicsBufferType type)
{
    switch (Renderer::GetInstance().GetApi())
    {
        case Renderer::API::OPEN_GL:
            return new VertexBufferOpenGL(size, data, type);
        break;
    }

    UASSERT(false, "Current API didn't implement vertex buffer");
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(int size, unsigned int* data, GraphicsBufferType type)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new IndexBufferOpenGL(size, data, type);
    break;
    }

    UASSERT(false, "Current API didn't implement index buffer");
    return nullptr;
}

VertexLayout* VertexLayout::Create(std::initializer_list<LayoutElement> initList)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new VertexLayoutOpenGL(initList);
    break;
    }

    UASSERT(false, "Current API didn't implement vertex layout");
    return nullptr;
}

RenderBuffer* RenderBuffer::Create(glm::uvec2 size, Format format)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new RenderBufferOpenGL(size, format);
        break;
    }

    UASSERT(false, "Current API didn't implement render buffer");
    return nullptr;
}

FrameBuffer* FrameBuffer::Create(glm::uvec2 size, bool enableDepth)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new FrameBufferOpenGL(size, enableDepth);
        break;
    }

    UASSERT(false, "Current API didn't implement frame buffer");
    return nullptr;
}

FrameBuffer* FrameBuffer::Create(Texture2D* attachedTexture)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new FrameBufferOpenGL(attachedTexture);
        break;
    }

    UASSERT(false, "Current API didn't implement frame buffer");
    return nullptr;
}

StorageBuffer* StorageBuffer::Create(void* data, size_t size)
{    
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new StorageBufferOpenGL(data, size);
        break;
    }
    
    UASSERT(false, "Current API didn't implement storage buffer");
    return nullptr;
}

AtomicCounterBuffer* AtomicCounterBuffer::Create(uint32_t* data, uint16_t size)
{
    switch (Renderer::GetInstance().GetApi())
    {
    case Renderer::API::OPEN_GL:
        return new AtomicCounterBufferOpenGL(data, size);
        break;
    }

    UASSERT(false, "Current API didn't implement atomic counter buffer");
    return nullptr;
}