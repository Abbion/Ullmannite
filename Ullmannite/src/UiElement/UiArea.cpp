#include "Ullpch.h"
#include "UiArea.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"

using namespace Ull;

UiArea::UiArea(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiElement(name, position, size)
{
    m_shader = ShaderManager::GetInstance()->GetShader(ShaderTag::UI_SHADER);
    CreateResources();
}

UiArea::~UiArea()
{

}

void UiArea::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    glm::vec2 pos = glm::vec2(m_position) * m_scale;
    glm::vec2 size = glm::vec2(m_position + m_size) * m_scale;

    float vertices[] = { 
        pos.x, pos.y, 0.0f,
        size.x, pos.y, 0.0f,
        pos.x, size.y, 0.0f,
        size.x, size.y, 0.0f
    };

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2 
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Position", GraphicsDataType::FLOAT, 3)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();


}

void UiArea::SetColor(const glm::vec4& color)
{
    m_color = color;
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

    m_shader->SetFloat4("ourColor", m_color);

    Renderer::GetInstance()->DrawElements(GraphicsRenderPrimitives::TRIANGLE, 6);
}