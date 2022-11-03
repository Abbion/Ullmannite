#include "Ullpch.h"
#include "glad/glad.h"
#include "BufferOpenGL.h"
#include "Logger/Logger.h"
#include "TextureOpenGL.h"
#include "DataConverterOpenGL.h"

using namespace Ull;

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

IndexBufferOpenGL::IndexBufferOpenGL(int size, unsigned int* data, GraphicsBufferType type) : m_size(size / sizeof(unsigned int))
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertUsageType(type));
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
    m_colorTarget->SetData(size, InternalDataFormat::RGB_32F, PixelDataFormat::RGB, GraphicsDataType::UBYTE, nullptr);
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

StorageBufferOpenGL::StorageBufferOpenGL(void* data, size_t size)
{
    m_size = size;

    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STREAM_READ);
}

StorageBufferOpenGL::~StorageBufferOpenGL()
{
    glDeleteBuffers(1, &m_bufferID);
}

void StorageBufferOpenGL::GetData(void* data, size_t size)
{
    glGetNamedBufferSubData(m_bufferID, 0, size, data);
}

void StorageBufferOpenGL::Bind(std::uint8_t bindIndex) const
{
    m_currentBindIndex = bindIndex;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindIndex, m_bufferID);
}

void StorageBufferOpenGL::Unbind() const
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_currentBindIndex, 0);
}

AtomicCounterBufferOpenGL::AtomicCounterBufferOpenGL(uint32_t* data, uint16_t size)
{
    m_size = size;

    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_bufferID);
    glBufferData(GL_ATOMIC_COUNTER_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

AtomicCounterBufferOpenGL::~AtomicCounterBufferOpenGL()
{
    glDeleteBuffers(1, &m_bufferID);
}

void AtomicCounterBufferOpenGL::Bind(std::uint8_t bindIndex) const
{
    m_currentBindIndex = bindIndex;
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, m_currentBindIndex, m_bufferID);
}

void AtomicCounterBufferOpenGL::Unbind() const
{
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, m_currentBindIndex, 0);
}