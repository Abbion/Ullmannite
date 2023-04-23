#include "Ullpch.h"
#include "Logger/Logger.h"
#include "UiElement.h"

using namespace Ull;

UiElement::UiElement(std::string name, glm::uvec2 position, glm::uvec2 size) : 
    m_name(name), m_position(position), m_size(size)
{
    m_modelMatrix = glm::mat4x4(1.0f);
}

Ull::UiElement::UiElement(UiElement&& source) :
    Drawable(std::move(source)),
    m_position{ source.m_position },
    m_size{ source.m_size },
    m_scale{ source.m_scale },
    m_name{ source.m_name },
    m_parent{ source.m_parent },
    m_children{ source.m_children }
{
}

UiElement::~UiElement()
{
    ULOGD("UiElement: " << m_name << " terminated!");
}

void UiElement::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    for(auto& child : m_children)
        child->HandleEvent(event);
}

void UiElement::AddUiElement(std::shared_ptr<UiElement> newElement)
{
    m_children.push_back(newElement);
}