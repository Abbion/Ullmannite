#include "Ullpch.h"
#include "glad/glad.h"
#include "BufferOpenGL.h"
#include "Logger/Logger.h"
#include "TextureOpenGL.h"

using namespace Ull;

namespace
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
}

VertexBufferOpenGL::VertexBufferOpenGL(int size, float* data, GraphicsBufferType type)
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, ConvertUsageType(type));
}

VertexBufferOpenGL::~VertexBufferOpenGL()
{
    glDeleteBuffers(1, &m_bufferID);
}

void VertexBufferOpenGL::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

void VertexBufferOpenGL::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBufferOpenGL::IndexBufferOpenGL(int size, unsigned int* data, GraphicsBufferType type) : m_size(size)
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, ConvertUsageType(type));
}

IndexBufferOpenGL::~IndexBufferOpenGL()
{
    glDeleteBuffers(1, &m_bufferID);
}

unsigned int IndexBufferOpenGL::GetSize() const
{
    return m_size;
}

void IndexBufferOpenGL::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void IndexBufferOpenGL::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


VertexLayoutOpenGL::VertexLayoutOpenGL(std::initializer_list<LayoutElement> initList) :
    m_elementList(std::move(initList))
{
    for (auto& element : m_elementList)
    {
        element.offset = m_totalSize;
        m_totalSize += SizeOfDataType(element.dataType) * element.amount;
    }
        
    glGenVertexArrays(1, &m_layoutID);
}

VertexLayoutOpenGL::~VertexLayoutOpenGL()
{
    glDeleteVertexArrays(1, &m_layoutID);
}

void VertexLayoutOpenGL::Build()
{
    if (!m_layoutCreated)
    {
        uint16_t index = 0;
        for (const auto& element : m_elementList)
        {
            glVertexAttribPointer(index, element.amount, ConvetDataType(element.dataType), element.normalized ? GL_TRUE : GL_FALSE, m_totalSize, (void*)element.offset);
            glEnableVertexAttribArray(index);
            ++index;
        }

        m_elementList.clear();
        m_layoutCreated = true;
    }
}

void VertexLayoutOpenGL::Bind() const
{
    glBindVertexArray(m_layoutID);
}

void VertexLayoutOpenGL::Unbind() const
{
    glBindVertexArray(0);
}

RenderBufferOpenGL::RenderBufferOpenGL(glm::uvec2 size, Format format)
{
    m_size = size;
    m_format = format;

    glGenRenderbuffers(1, &m_bufferID);
    Bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.x, m_size.y);
}

RenderBufferOpenGL::~RenderBufferOpenGL()
{
    glDeleteRenderbuffers(1, &m_bufferID);
}

void RenderBufferOpenGL::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_bufferID);
}

void RenderBufferOpenGL::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBufferOpenGL::FrameBufferOpenGL(glm::uvec2 size, bool enableDepth)
{
    glGenFramebuffers(1, &m_bufferID);
    Bind();

    //Attach color target
    m_colorTarget = new Texture2DOpenGL();
    m_colorTarget->SetData(size, ColorFormat::RGB, ColorFormat::RGB, nullptr);
    m_colorTarget->SetSampling(Sampling::LINEAR, Sampling::LINEAR);

    auto txId = static_cast<Texture2DOpenGL*>(m_colorTarget)->GetOpenGLTextureID();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, txId, 0);

    //Attach depth target
    if(enableDepth)
    {
        m_depthTarget = new RenderBufferOpenGL(size, RenderBuffer::Format::DEPTH_STENCIL);
        auto rbo = static_cast<RenderBufferOpenGL*>(m_depthTarget)->GetOpenGLBufferID();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        ULOGE("Framebuffer is not complete!");

    Unbind();
}

FrameBufferOpenGL::~FrameBufferOpenGL()
{
    delete m_colorTarget;

    if(m_depthTarget != nullptr)
        delete m_depthTarget;

    glDeleteFramebuffers(1, &m_bufferID);
}

void FrameBufferOpenGL::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
}

void FrameBufferOpenGL::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}