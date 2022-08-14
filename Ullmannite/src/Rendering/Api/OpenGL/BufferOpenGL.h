#pragma once
#include "Rendering/Api/Buffer.h"
#include "glad/glad.h"

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
        
        void Bind() const override;
        void Unbind() const override;

    private:
        unsigned int m_bufferID{ 0 };
    };

    class VertexLayoutOpenGL : public VertexLayout
    {
    public:
        VertexLayoutOpenGL(std::initializer_list<LayoutElement> initList);
        ~VertexLayoutOpenGL();

        virtual void Build() const override;
        void Bind() const override;
        void Unbind() const override;
    
    private:
        unsigned int m_layoutID{ 0 };
        mutable bool m_layoutCreated{ false };
        uint32_t m_totalSize{ 0 };
        std::vector<LayoutElement> m_elementList;
    };
}