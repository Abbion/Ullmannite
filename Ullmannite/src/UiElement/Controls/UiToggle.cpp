#include "Ullpch.h"
#include "UiToggle.h"
#include "Rendering/Api/Renderer.h"

using namespace Ull;

UiToggle::UiToggle(std::string name, glm::uvec2 position, glm::uvec2 size) :
	UiBasicControl(name, position, size, UiControlType::UiToggle),
    m_buttonText{ std::make_shared<UiText>(std::string(name + "Text"), glm::uvec2(0, 0), size, L"Toggle") }
{
    m_buttonText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
    m_buttonText->SetSampleThreshold(1.5f);

    AddChildNode(m_buttonText);
}

void UiToggle::SetSize(const glm::uvec2 size)
{
    UiBasicControl::SetSize(size);
    m_buttonText->SetSize(size);
}

void UiToggle::HandleEvent(Event* event)
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
        {
            m_state = !m_state;
            m_state == true ? m_onEnabled(*this) : m_onDisabled(*this);
        }
        m_pressed = false;
        break;

    default:
        break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiToggle::Update()
{
    UiBasicControl::Update();
}

void UiToggle::Render()
{
    Renderer::GetInstance().SetBlending(true);
    UiBasicControl::Render();
    Renderer::GetInstance().SetBlending(false);
}