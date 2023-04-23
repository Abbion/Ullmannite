#include "Ullpch.h"
#include "UiToggle.h"

using namespace Ull;

UiToggle::UiToggle(std::string name, glm::uvec2 position, glm::uvec2 size) :
	UiBasicControl(name, position, size)
{

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
    UiBasicControl::Render();
}