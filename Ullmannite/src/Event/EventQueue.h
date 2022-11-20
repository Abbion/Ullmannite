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

        void PushEvent(const std::shared_ptr<Event>& newEvent);
        std::shared_ptr<Event> PopEvent();
        
        void CheckPublishedEvents();

        bool HasPenddingEvents() const;

        unsigned int GetSize() const;
        
        void MakeEventUnique(EventType eventType);
        void ClearEventQueue();

    private:
        std::deque<std::shared_ptr<Event>> m_events;
    };
};
