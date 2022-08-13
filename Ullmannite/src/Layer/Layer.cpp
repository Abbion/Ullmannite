#include "Ullpch.h"
#include "Layer/Layer.h"
#include "Event/Event.h"

using namespace Ull;

Layer::Layer(std::string name) : m_name(name)
{
    
}

Layer::~Layer()
{
    delete m_layout;
    
    ULOGD("Layer " << m_name << " terminated");
}

void Layer::CreateLayout(glm::uvec2 size)
{
    auto layoutName = m_name + " layout";
    m_layout = new UiArea(layoutName, glm::uvec2(0, 0), size);
}

void Layer::HandleEvent(Event* event)
{
    if(event->GetType() == EventType::KeyDown)
    {
        ULOGD(m_name << " key down");
        event->MarkHandeled(true);
    }       
}