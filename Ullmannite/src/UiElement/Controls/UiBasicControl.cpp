#include "Ullpch.h"
#include "UiBasicControl.h"
#include "Application/Application.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

UiBasicControl::UiBasicControl(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const UiControlType type) :
    UiElement(name, position, size, UiElementType::Control),
    m_uiControlType{ type }
{
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();
    m_shader = shaderManager.GetShader(ShaderTag::UI_BASIC_COLOR);
}

void UiBasicControl::SetFunctionality(UiControlFunctionality functionality, State state)
{
    if (state == State::Enable)
        m_functionality |= static_cast<Functionality>(functionality);
    else
        m_functionality &= (~static_cast<Functionality>(functionality));
}

bool UiBasicControl::IsFunctionalitySet(UiControlFunctionality functionality)
{
    return m_functionality & static_cast<Functionality>(functionality);
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
        if (m_functionality & static_cast<Functionality>(UiControlFunctionality::Hover))
            CheckHover();
    break;

    case EventType::WindowResize:
    case EventType::UiScaledDown:
    case EventType::UiScaledUp:
    case EventType::RenderAreaSizeChanged:
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
    m_shader->Bind();

    m_shader->SetFloat4("color", m_hover ? m_hoverColor : m_backgroundColor);
    m_shader->SetFloat4x4("modelMatrix", m_perspective * GetTransform());

    m_layout->Bind();

    Renderer::GetInstance().DrawElements(GraphicsRenderPrimitives::TRIANGLE, m_indexBuffer->GetSize());

    UiElement::Render();
}

void UiBasicControl::CheckHover()
{
    // The reander area creates a frame buffer. Every object in a render area is relative to that area.
    // If the object has the position 0,0 on screen this object will be offeted by the render area.
    // We have to calculate the offset and subtract it from the cursor position.

    const auto mousePosition = Application::GetMouse().GetMousePosition();
    glm::ivec2 renderAreaOffset = { 0, 0 };

    auto parent = GetParent();
    while (parent != nullptr)
    {
        if (parent->GetType() == UiElementType::RenderArea)
            renderAreaOffset += parent->GetPosition();
        parent = parent->GetParent();
    }
    
    if (PointInStaticRect<glm::ivec2>(mousePosition - renderAreaOffset, GetPosition(), GetSize()))
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
    const auto renderArea = FindUiElementAboveByType(UiElementType::RenderArea);

    if (renderArea)
    {
        const auto renderAreaSize = renderArea.value()->GetSize();
        m_perspective = glm::ortho(0.0f, (float)renderAreaSize.x, (float)renderAreaSize.y, 0.0f);
    }
}