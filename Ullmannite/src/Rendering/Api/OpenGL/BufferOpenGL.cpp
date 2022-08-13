#include "Ullpch.h"
#include "BufferOpenGL.h"

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
            //TODO ERROR HANDLING
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
            //TODO ERROR HANDLING
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
            //TODO ERROR HANDLING
            break;
        }
    }
}

VertexBufferOpenGL::VertexBufferOpenGL(int size, float* data, GraphicsBufferType type)
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    glBufferData(m_bufferID, size, data, ConvertUsageType(type));
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


IndexBufferOpenGL::IndexBufferOpenGL(int size, int* data, GraphicsBufferType type)
{
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertUsageType(type));
}

IndexBufferOpenGL::~IndexBufferOpenGL()
{
    glDeleteBuffers(1, &m_bufferID);
}

void IndexBufferOpenGL::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void IndexBufferOpenGL::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


VertexLayoutOpenGL::VertexLayoutOpenGL(std::initializer_list<VertexLayoutElement> initList) :
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

void VertexLayoutOpenGL::Bind() const
{
    glBindVertexArray(m_layoutID);
    
    if(!m_layoutCreated)
    {
        uint8_t index = 0;
        for(const auto& element : m_elementList)
        {
            glVertexAttribPointer(index, element.amount, ConvetDataType(element.dataType), element.normalized ? GL_TRUE : GL_FALSE, m_totalSize, (void*)element.offset);
            glEnableVertexAttribArray(index);
            index++;
        }

        m_layoutCreated = true;
    }
}