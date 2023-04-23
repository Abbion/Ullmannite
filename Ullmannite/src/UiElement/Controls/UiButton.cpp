#include "Ullpch.h"
#include "UiButton.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

UiButton::UiButton(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiBasicControl(name, position, size)
{

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
    UiBasicControl::Render();
}
