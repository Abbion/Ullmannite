#pragma once
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Event/EventHandler.h"
#include "UiElement/UiElement.h"

namespace Ull
{
    class Layer : public EventHandler
    {
    public:
        Layer(std::string name);
        virtual ~Layer();

        void HandleEvent(Event* event) override;
        
        void SetBlockEventPassing(bool blockEventPassing)   { m_blockEventPassing = blockEventPassing; }    //TODO: Delete This event is doing all the lifting
        
        bool BlockEventPassing() const      { return m_blockEventPassing; }
        std::string GetName() const         { return m_name; }

    private:
        bool m_blockEventPassing{ false };
        std::string m_name;

        //std::vector<UiElement> m_uiElements;
        unsigned int m_focusedElement { 0 };
    };
};