#pragma once
#include "Ullpch.h"
#include "GraphicsTypes.h"

namespace Ull
{
    struct VertexLayoutElement
    {
        std::string name;
        GraphicsDataType dataType;
        uint8_t amount;
        bool normalized;
        uint16_t offset;

         VertexLayoutElement(std::string name, GraphicsDataType dataType, uint8_t amount, bool normalized = false)
         : name(name), dataType(dataType), amount(amount), normalized(normalized) {}
    };

    class VertexBuffer
    {
        public:
            NON_COPYABLE(VertexBuffer)

            virtual ~VertexBuffer() {}

            static VertexBuffer* Creeate(int size, float* data, GraphicsBufferType type);

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

            static IndexBuffer* Create(int size, int* data, GraphicsBufferType type);

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

            static VertexLayout* Create(std::initializer_list<VertexLayoutElement> initList);

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;
    
        protected:
            VertexLayout() = default;
    };
}
