#pragma once
#include "Ullpch.h"
#include "GraphicsTypes.h"
#include "Texture.h"

namespace Ull
{
    struct LayoutElement
    {
        std::string name;
        GraphicsDataType dataType;
        uint16_t amount;
        bool normalized;
        uint32_t offset{ 0 };

         LayoutElement(std::string name, GraphicsDataType dataType, uint16_t amount, bool normalized = false)
         : name(name), dataType(dataType), amount(amount), normalized(normalized) {}
    };

    class VertexBuffer
    {
        public:
            NON_COPYABLE(VertexBuffer)

            virtual ~VertexBuffer() {}

            static VertexBuffer* Create(int size, float* data, GraphicsBufferType type);

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

        protected:
            VertexBuffer() = default;
    };

    class IndexBuffer
    {
        public:
            NON_COPYABLE(IndexBuffer);

            virtual ~IndexBuffer() {}

            static IndexBuffer* Create(int size, unsigned int* data, GraphicsBufferType type);

            virtual unsigned int GetSize() const = 0;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

        protected:
            IndexBuffer() = default;
    };

    class VertexLayout
    {
        public:
            NON_COPYABLE(VertexLayout);
        
            virtual ~VertexLayout() {}

            static VertexLayout* Create(std::initializer_list<LayoutElement> initList);

            virtual void Build() = 0;
            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;
    
        protected:
            VertexLayout() = default;
    };

    class RenderBuffer
    {
    public:
        enum class Format {
            DEPTH,
            DEPTH_STENCIL
        };

    public:
        //Allow for copy
        virtual ~RenderBuffer() {}

        glm::uvec2 GetSize() { return m_size; }
        Format GetFormat() { return m_format; }

        static RenderBuffer* Create(glm::uvec2 size, Format format);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

    protected:
        RenderBuffer() = default;
        
        glm::uvec2 m_size{ 0, 0 };
        Format m_format{ Format::DEPTH_STENCIL };
    };

    class FrameBuffer
    {
    public:
        NON_COPYABLE(FrameBuffer);

        virtual ~FrameBuffer() {}

        Texture2D* GetColorTarget() const { return m_colorTarget; }
        RenderBuffer* GetDepthTarget() const { return m_depthTarget; }
        
        static FrameBuffer* Create(glm::uvec2 size, bool enableDepth);
        static FrameBuffer* Create(Texture2D* attachedTexture);
        virtual void GetPixels(const glm::uvec2 position, const glm::uvec2 size, const PixelDataFormat pixelDataFormat, const GraphicsDataType dataType, void* data) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    protected:
        FrameBuffer() = default;
        
        Texture2D* m_colorTarget{ nullptr };
        RenderBuffer* m_depthTarget{ nullptr };
    };

    class StorageBuffer
    {
    public:
        NON_COPYABLE(StorageBuffer);

        virtual ~StorageBuffer() {}

        size_t GetSize() { return m_size; }

        static StorageBuffer* Create(void* data, size_t size);

        virtual void GetData(void* data, size_t size) = 0;

        virtual void Bind(std::uint8_t bindIndex) const = 0;
        virtual void Unbind() const = 0;
    protected:
        StorageBuffer() = default;

        size_t m_size{ 0 };
        mutable std::uint8_t m_currentBindIndex{ 0 };
    };

    class AtomicCounterBuffer
    {
    public:
        NON_COPYABLE(AtomicCounterBuffer);

        virtual ~AtomicCounterBuffer() {}

        static AtomicCounterBuffer* Create(uint32_t* data, uint16_t size);

        virtual void Bind(std::uint8_t bindIndex) const = 0;
        virtual void Unbind() const = 0;

    protected:
        AtomicCounterBuffer() = default;
        uint16_t m_size{ 0 };
        mutable std::uint8_t m_currentBindIndex{ 0 };
    };
}