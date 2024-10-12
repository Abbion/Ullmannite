#include "Ullpch.h"
#include "UiBasicControl.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

UiBasicControl::UiBasicControl(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiElement(name, position, size)
{
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_BASIC_COLOR);
}

void UiBasicControl::SetParent(NotOwner<UiElement> parent)
{
    //UiElement::SetParent(parent);
    //UpdatePerspective();
}

void UiBasicControl::CreateResources()
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;

    if (m_indexBuffer != nullptr)
        delete m_indexBuffer;

    if (m_layout != nullptr)
        delete m_layout;

    float vertices[] = { 
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = { 
        0, 1, 2,
        1, 3, 2 
    };

    m_layout = VertexLayout::Create({
        LayoutElement("Vertices", GraphicsDataType::FLOAT, 3)
    });

    m_layout->Bind();

    m_vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices, GraphicsBufferType::STATIC_DRAW);
    m_indexBuffer = IndexBuffer::Create(sizeof(indices), indices, GraphicsBufferType::STATIC_DRAW);

    m_layout->Build();
    m_vertexBuffer->Unbind();
    m_layout->Unbind();
}

void UiBasicControl::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::MouseMove:
        CheckHover();
    break;

    case EventType::WindowResize:
    case EventType::UiScaledDown:
    case EventType::UiScaledUp:
    case EventType::ParentPositionChanged:
    case EventType::ParentSizeChanged:
        UpdatePerspective();
    break;

    case EventType::WindowLostFocus:
    case EventType::WindowMinimized:
    case EventType::MouseExitedWindow:
        m_hover = false;
    break;

    default:
        break;
    }

    UiElement::HandleEvent(event);
}

void UiBasicControl::Update()
{
    UiElement::Update();
}

void UiBasicControl::Render()
{
    auto parent = GetParent();
    auto position = GetPosition();
    const auto size = GetSize();

    while (parent != nullptr)
    {
        position += parent->GetPosition();
        parent = parent->GetParent();
    }

    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3{ position.x, position.y, 0.0f });
    transform = glm::scale(transform, glm::vec3{ size.x, size.y, 1.0f });

    m_shader->Bind();

    m_shader->SetFloat4("color", m_hover ? m_hoverColor : m_backgroundColor);
    m_shader->SetFloat4x4("modelMatrix", m_perspective * transform);

    m_layout->Bind();

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    UiElement::Render();
}

void UiBasicControl::CheckHover()
{
    if (PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize()))
    {
        m_hover = true;
    }
    else
    {
        m_hover = false;
    }
}

inline void UiBasicControl::UpdatePerspective()
{
    auto parent = GetParent();

    UASSERT((parent != nullptr), "No parent assigned!");

    const auto parentSize = static_cast<Object2D*>(parent.Get())->GetSize();

    if (parentSize.x == 45)
    {
        m_perspective = glm::ortho(0.0f, 1280.0f, 30.0f, 0.0f);
    }
    else
        m_perspective = glm::ortho(0.0f, (float)parentSize.x, (float)parentSize.y, 0.0f);
}