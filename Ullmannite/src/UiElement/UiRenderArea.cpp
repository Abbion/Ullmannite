#include "Ullpch.h"
#include "UiRenderArea.h"
#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Utilities/CollisionCheckers.h"

using namespace Ull;

UiRenderArea::UiRenderArea(std::string name, glm::uvec2 position, glm::uvec2 size, bool usesDepth) :
    UiElement(name, position, size, UiElementType::Area),
    m_usesDepth(usesDepth)
{
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_BASIC_COLOR);
    CreateResources();
}

UiRenderArea::~UiRenderArea()
{
    if (m_frameBuffer != nullptr)
        delete m_frameBuffer;
}

void UiRenderArea::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    CreateFrameBuffer();

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

    m_areaUpdated = true;
}

void UiRenderArea::BindTargetTexture()
{
    m_frameBuffer->GetColorTarget()->Bind();
}

void UiRenderArea::SetBackgroundColor(const glm::vec4& color)
{
    m_color = color;
    m_areaUpdated = true;
}

void UiRenderArea::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::MouseMove:
    case EventType::MouseEnteredWindow:
        CheckMouseInArea();
    break;

    case EventType::MouseExitedWindow:
        m_areaUpdated = true;
        m_inArea = false;
    break;

    default:
        break;
    }

    UiElement::HandleEvent(event);
}

void UiRenderArea::Update()
{
    UiElement::Update();
}

void UiRenderArea::Render()
{
    //if(m_areaUpdated)
    {
        m_frameBuffer->Bind();

        RenderBackground();
        UiElement::Render();

        m_frameBuffer->Unbind();

        m_areaUpdated = false;
    }
}

void UiRenderArea::RenderBackground()
{
    Renderer::GetInstance().Clear(Renderer::ClearBits::COLOR);
    Renderer::GetInstance().SetViewPort(glm::ivec2(0, 0), GetSize());

    m_shader->Bind();

    m_shader->SetFloat4("color", m_color);
    m_shader->SetFloat4x4("modelMatrix", glm::mat4(1.0f));

    m_layout->Bind();

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());
}

void UiRenderArea::CheckMouseInArea()
{
    if (PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize()))
    {
        m_areaUpdated = true;
        m_inArea = true;
    }
    else
    {
        if (m_inArea = true)
            m_areaUpdated = true;
        m_inArea = false;
    }
}

void UiRenderArea::CreateFrameBuffer()
{
    if (m_frameBuffer != nullptr)
        delete m_frameBuffer;

    m_frameBuffer = FrameBuffer::Create(GetSize(), m_usesDepth);
}
