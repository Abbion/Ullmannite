#include <Ullpch.h>
#include "EventQueue.h"
#include "Logger/Logger.h"

using namespace Ull;

EventQueue::EventQueue()
{
}

EventQueue::~EventQueue()
{
    ClearEventQueue();
    ULOGD("EventQueue terminated");
}

void EventQueue::PushEvent(const std::shared_ptr<Event>& newEvent)
{
    m_events.push_front(newEvent);
}

std::shared_ptr<Event> EventQueue::PopEvent()
{
    auto lastEvent = m_events.back();
    m_events.pop_back();
    return lastEvent;
}


unsigned int EventQueue::GetSize() const
{
    return static_cast<unsigned int>(m_events.size());
}

void EventQueue::MakeEventUnique(EventType eventType)
{
    std::deque<std::shared_ptr<Event>> uniqueQueue;
    bool lock = false;

    for(const auto& currentEvent : m_events)
    {
        if(currentEvent->GetType() == eventType)
        {
            if(!lock)
            {
                uniqueQueue.push_back(currentEvent);
                lock = true;
            }
        }
        else
            uniqueQueue.push_back(currentEvent);
    }

    ClearEventQueue();
    m_events = std::move(uniqueQueue);
}

void EventQueue::ClearEventQueue()
{
   m_events.clear();
}