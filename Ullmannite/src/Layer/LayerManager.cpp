#include "Ullpch.h"
#include "Layer/LayerManager.h"

#include "Logger/Logger.h"

using namespace Ull;

LayerManager::LayerManager()
{
}

LayerManager::~LayerManager()
{
    m_layers.clear();
    ULOGD("Layer manager terminated");
}

void LayerManager::PushLayer(std::shared_ptr<Layer> newLayer)
{
    m_layers.push_back(newLayer);
}

void LayerManager::PopLayer()
{
    m_layers.pop_back();
}

void LayerManager::HandleEvent(Event* event)
{
    for(auto & layer : m_layers)
    {
        layer->HandleEvent(event);

        if(event->IsHandeled())
            break;
    }
}

std::shared_ptr<Layer> LayerManager::GetTopLayer()
{
    UASSERT(m_layers.size() != 0, "Layer queue is empty");

    return m_layers.back();
}

unsigned int LayerManager::GetSize() const
{
    return static_cast<unsigned int>(m_layers.size());
}