#pragma once
#include "Event.h"
#include <deque>
#include <mutex>

namespace Ull
{
    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();

        void PushEvent(const std::shared_ptr<Event>& newEvent);

        std::shared_ptr<Event> PopEvent();
        
        bool HasPenddingEvents() const;
        unsigned int GetSize() const;
        
        void MakeEventUnique(EventType eventType);
        void ClearEventQueue();

        inline void LockAccess() { m_mutex.lock(); }
        inline void UnlockAccess() { m_mutex.unlock(); }

    private:
        std::deque<std::shared_ptr<Event>> m_events;
        std::mutex m_mutex;
    };
};
