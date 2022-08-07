#include "Ullpch.h"
#include "Layer/Layer.h"
#include "Event/Event.h"

using namespace Ull;

Layer::Layer(std::string name) : m_name(name)
{

}

Layer::~Layer()
{
    ULOGD("Layer " << m_name << " terminated");
}

void Layer::HandleEvent(Event* event)
{
    if(event->GetType() == EventType::KeyDown)
    {
        ULOGD(m_name << " key down");
        event->MarkHandeled(true);
    }       
}