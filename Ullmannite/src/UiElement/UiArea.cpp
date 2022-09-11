#include "Ullpch.h"
#include "UiArea.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

UiArea::UiArea(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiElement(name, position, size)
{
    m_shader = ShaderManager::GetInstance()->GetShader(ShaderTag::UI_SHADER);
    CreateResources();
}

UiArea::~UiArea()
{
    if (m_frameBuffer != nullptr)
        delete m_frameBuffer;
}

void UiArea::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    m_frameBuffer = FrameBuffer::Create(m_size);

    //Abstract this to a plane
    float vertices[] = { 
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
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

void UiArea::BindTargetTexture()
{
    m_frameBuffer->GetColorTarget()->Bind();
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
    m_frameBuffer->Bind();

    Renderer::GetInstance()->Clear(Renderer::ClearBits::COLOR);
    Renderer::GetInstance()->SetViewPort(glm::ivec2(0, 0), m_size);

    m_shader->Bind();

    m_shader->SetFloat4("ourColor", m_color);
    
    m_layout->Bind();

    Renderer::GetInstance()->DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
    
    m_frameBuffer->Unbind();
}