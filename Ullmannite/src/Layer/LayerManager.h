#pragma once
#include <memory>
#include "Event/EventHandler.h"
#include "Event/Event.h"
#include <deque>

namespace Ull
{
    class Layer;

    class LayerManager : public EventHandler
    {
    public:
        LayerManager();
        ~LayerManager();

        void PushLayer(std::shared_ptr<Layer> newLayer);
        void DropAllLayers();
        std::deque<std::shared_ptr<Layer>>& GetLayers() { return m_layers; }
        std::shared_ptr<Layer> GetLayerByName(const std::string& layerName);

        unsigned int GetSize() const;

        void HandleEvent(Event* event) override;
        
    private:
        std::deque<std::shared_ptr<Layer>> m_layers;
    };
};