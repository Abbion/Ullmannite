#pragma once
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Event/EventHandler.h"
#include "UiElement/UiArea.h"

namespace Ull
{
    class Layer : public EventHandler
    {
    public:
        Layer(std::string name);
        virtual ~Layer();

        void CreateLayout(glm::uvec2 size);

        void HandleEvent(Event* event) override;
        
        std::string GetName() const { return m_name; }

    private:
        std::string m_name;

        UiArea* m_layout;
        unsigned int m_focusedElement { 0 };
    };
};