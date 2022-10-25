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

        void Bind() const override;
        void Unbind() const override;
    
    private:
        unsigned int m_bufferID{ 0 };
    };

    template<typename T>
    class StorageBufferOpenGL : public StorageBuffer<T>
    {
    public:
        StorageBufferOpenGL(T* data, size_t size)
        {
            glGenBuffers(1, &m_bufferID);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
            glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_COPY);
        }

        virtual ~StorageBufferOpenGL() 
        {
            glDeleteBuffers(1, &m_bufferID);
        }

        void Bind() const override 
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
        }
        void Unbind() const override
        {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }
    
    private:
        unsigned int m_bufferID{ 0 };
    };
}