#pragma once
#include "Event.h"
#include <deque>

namespace Ull
{
    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void PushEvent(const Event& newEvent);
        void PushEvent(Event&& newEvent);

        Event PopEvent();
        unsigned int GetSize() const;
        
        void MakeEventUnique(EventType eventType);
        void ClearEventQueue();

    private:
        std::deque<Event> m_events;
    };
};
