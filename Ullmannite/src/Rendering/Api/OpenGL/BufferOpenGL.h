#pragma once
#include "Rendering/Api/Buffer.h"

namespace Ull
{
    class VertexBufferOpenGL : public VertexBuffer
    {
    public:
        VertexBufferOpenGL(int size, float* data, GraphicsBufferType type);
        ~VertexBufferOpenGL();

        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
    };

    class IndexBufferOpenGL : public IndexBuffer
    {
    public:
        IndexBufferOpenGL(int size, unsigned int* data, GraphicsBufferType type);
        ~IndexBufferOpenGL();
        
        unsigned int GetSize() const override;

        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
        unsigned int m_size{ 0 };
    };

    class VertexLayoutOpenGL : public VertexLayout
    {
    public:
        VertexLayoutOpenGL(std::initializer_list<LayoutElement> initList);
        ~VertexLayoutOpenGL();

        void Build() override;
        void Bind() const override;
        void Unbind() const override;
    
    private:
        unsigned int m_layoutID{ 0 };
        bool m_layoutCreated{ false };
        uint32_t m_totalSize{ 0 };
        std::vector<LayoutElement> m_elementList;
    };

    class RenderBufferOpenGL : public RenderBuffer
    {
    public:
        RenderBufferOpenGL(glm::uvec2 size, Format format);
        ~RenderBufferOpenGL();

        unsigned int GetOpenGLBufferID() const { return m_bufferID; }

        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
    };

    class FrameBufferOpenGL : public FrameBuffer
    {
    public:
        FrameBufferOpenGL(glm::uvec2 size, bool enableDepth);
        ~FrameBufferOpenGL();

        unsigned int GetOpenGLBufferID() const { return m_bufferID; }

        void Bind() const override;
        void Unbind() const override;
    
    private:
        unsigned int m_bufferID{ 0 };
    };

    class StorageBufferOpenGL : public StorageBuffer
    {
    public:
        StorageBufferOpenGL(void* data, size_t size);
        virtual ~StorageBufferOpenGL();

        unsigned int GetOpenGLBufferID() const { return m_bufferID; }
        void GetData(void* data, size_t size) override;

        void Bind(std::uint8_t bindIndex) const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
    };

    class AtomicCounterBufferOpenGL : public AtomicCounterBuffer
    {
    public: 
        AtomicCounterBufferOpenGL(uint32_t* data, uint16_t size);
        virtual ~AtomicCounterBufferOpenGL();

        unsigned int GetOpenGLBufferID() const { return m_bufferID; }

        void Bind(std::uint8_t bindIndex) const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
    };
}