#pragma
#include <memory>
#include "Event/EventHandler.h"
#include "Layer/Layer.h"
#include "Event/Event.h"
#include <deque>

namespace Ull
{
    class LayerManager : public EventHandler
    {
    public:
        LayerManager();
        ~LayerManager();

        void PushLayer(std::shared_ptr<Layer> newLayer);
        void PopLayer();

        void HandleEvent(Event* event) override;

        std::shared_ptr<Layer> GetTopLayer();
        std::deque<std::shared_ptr<Layer>>& GetAllLayers() { return m_layers; }
        unsigned int GetSize() const;
        
    private:
        std::deque<std::shared_ptr<Layer>> m_layers;
    };
};