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

        std::shared_ptr<Layer> GetTopLayer();
        std::deque<std::shared_ptr<Layer>>& GetAllLayers() { return m_layers; }
        unsigned int GetSize() const;

        void HandleEvent(Event* event) override;
        
    private:
        std::deque<std::shared_ptr<Layer>> m_layers;
    };
};