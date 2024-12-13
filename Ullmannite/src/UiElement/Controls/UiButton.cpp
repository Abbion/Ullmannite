#include "Ullpch.h"
#include "UiButton.h"
#include "Rendering/Api/Renderer.h"

using namespace Ull;

UiButton::UiButton(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiBasicControl(name, position, size, UiControlType::UiButton),
    m_buttonText{ std::make_shared<UiText>(std::string(name + "Text"), glm::uvec2(0, 0), size, L"Button") }
{
    m_buttonText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
    m_buttonText->SetSampleThreshold(1.5f);
    //m_buttonText.SetSampleThreshold(100.f);

    AddChildNode(m_buttonText);
}

void UiButton::SetSize(const glm::uvec2 size)
{
    UiBasicControl::SetSize(size);
    m_buttonText->SetSize(size);
}

void UiButton::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::MouseDown:
        CheckHover();
        if (m_hover)
            m_pressed = true;
    break;

    case EventType::MouseUp:
        CheckHover();
        if (m_pressed && m_hover)
            m_onClick(*this);
        m_pressed = false;
    break;

    default:
    break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiButton::Update()
{
    UiBasicControl::Update();
}

void UiButton::Render()
{
    Renderer::GetInstance().SetBlending(true);
    UiBasicControl::Render();
    Renderer::GetInstance().SetBlending(false);
}
