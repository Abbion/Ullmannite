#include "Ullpch.h"
#include "UiArea.h"
#include "Rendering/Converters/CoordinateConverter.h"
#include "Rendering/Api/Renderer.h"

using namespace Ull;

UiArea::UiArea(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiElement(name, position, size)
{
    CreateResources();
}

void UiArea::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    auto cc = CoordinateConverter::GetInstance();
    auto pos = cc->ScreenToNormal(m_position) * m_scale;
    auto siz = cc->ScreenToNormal(m_size) * m_scale;

    float vertices[] = { 
        pos.x, pos.y, 0.0f,
        siz.x, pos.y, 0.0f,
        pos.x, siz.y, 0.0f,
        siz.x, siz.y, 0.0f 
    };

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2 
    };

    //TODO: Move this out of here!!!!
    m_layout = VertexLayout::Create({
        LayoutElement("Position", GraphicsDataType::FLOAT, 3)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();

    //TODO: Move this out of here!!!!
    m_shader = Shader::Create("TestVertex", "TestPixel");
}

void UiArea::HandleEvent(Event* event)
{

}

void UiArea::Update()
{

}

void UiArea::Render()
{
    m_shader->Bind();
    m_layout->Bind();
    Renderer::GetInstance()->DrawElements(GraphicsRenderPrimitives::TRIANGLE, 6);
}