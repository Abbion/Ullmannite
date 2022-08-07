#pragma once
#include "Event/Event.h"

namespace Ull
{
    class EventHandler
    {
    public: 
        EventHandler() {}
        virtual ~EventHandler() {}

        virtual void HandleEvent(Event* event) = 0;
    };
}