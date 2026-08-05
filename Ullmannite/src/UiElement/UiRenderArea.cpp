#include "Ullpch.h"
#include "UiRenderArea.h"
#include "Application/Application.h"
#include "Rendering/Api/ShaderManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Utilities/CollisionCheckers.h"

using namespace Ull;

UiRenderArea::UiRenderArea(const std::string& name, const glm::uvec2 position, const glm::uvec2 size, const bool usesDepth) :
    UiElement(name, position, size, UiElementType::RenderArea),
    m_usesDepth(usesDepth)
{
    SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    CreateFrameBuffer();
}

void UiRenderArea::BindTargetTexture()
{
    m_frameBuffer->GetColorTarget()->Bind();
}

void UiRenderArea::OnSizeChange()
{
    CreateFrameBuffer();
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

        Clear();
        UiElement::Render();

        m_frameBuffer->Unbind();

        m_areaUpdated = false;
    }
}

void UiRenderArea::Clear()
{
    const uint8_t clearDepthBit = m_usesDepth ? Renderer::ClearBits::DEPTH : 0u;

    Application::GetRenderer().SetClearColor(m_color);
    Application::GetRenderer().Clear(Renderer::ClearBits::COLOR | clearDepthBit);
    Application::GetRenderer().SetViewPort(glm::ivec2(0, 0), GetSize());
}

void UiRenderArea::CheckMouseInArea()
{
    if (PointInStaticRect<glm::ivec2>(Application::GetMouse().GetMousePosition(), GetPosition(), GetSize()))
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
