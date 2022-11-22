#include "Ullpch.h"
#include "Logger/Logger.h"
#include "UiElement.h"

using namespace Ull;

UiElement::UiElement(std::string name, glm::uvec2 position, glm::uvec2 size) : 
    m_name(name), m_position(position), m_size(size)
{
    m_modelMatrix = glm::mat4x4(1.0f);
}

UiElement::~UiElement()
{
    for(auto& child : m_children)
        delete child;

    ULOGD("UiElement: " << m_name << " terminated!");
}

void UiElement::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    for(auto& child : m_children)
        child->HandleEvent(event);
}

void UiElement::AddUiElement(UiElement* newElement)
{
    m_children.push_back(newElement);
}