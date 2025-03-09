#include "Ullpch.h"
#include "Layer/LayerManager.h"
#include "Logger/Logger.h"
#include "Layer/Layer.h"

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
    m_layers.emplace_back(newLayer);
}

void LayerManager::DropAllLayers()
{
    m_layers.clear();
}

unsigned int LayerManager::GetSize() const
{
    return static_cast<unsigned int>(m_layers.size());
}

void LayerManager::HandleEvent(Event* event)
{
    if(event->IsHandeled())
        return;


    for (auto layerItr = m_layers.rbegin(); layerItr != m_layers.rend(); layerItr++)
        (*layerItr)->HandleEvent(event);

    //for(auto & layer : m_layers)
      //  layer->HandleEvent(event);
}