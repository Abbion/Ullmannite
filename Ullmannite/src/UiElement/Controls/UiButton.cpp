#include "Ullpch.h"
#include "UiButton.h"
#include "Application/Application.h"


using namespace Ull;

UiButton::UiButton(std::string name, glm::vec2 position, glm::vec2 size) :
    UiBasicControl(name, position, size, UiControlType::UiButton),
    m_buttonText{ std::make_shared<UiText>(std::string(name + "Text"), glm::vec2(0, 0), size, L"Button") }
{
    m_buttonText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
    m_buttonText->SetSampleThreshold(1.5f);

    AddChildNode(m_buttonText);
}

void UiButton::SetSize(const glm::vec2 size)
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
        if (IsVisible() == false)
            return;

        CheckHover();
        if (m_hover)
        {
            m_pressed = true;
            event->MarkHandeled(true);
        }
    break;

    case EventType::MouseUp:
        if (IsVisible() == false)
            return;

        CheckHover();
        if (m_pressed && m_hover)
        {
            m_onClick(*this);
            event->MarkHandeled(true);
        }
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
    if (!IsVisible())
        return;

    Application::GetRenderer().SetBlending(Renderer::State::ENABLE);
    UiBasicControl::Render();
    Application::GetRenderer().SetBlending(Renderer::State::DISABLE);
}
