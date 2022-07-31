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

void EventQueue::PushEvent(const Event& newEvent)
{
    m_events.push_front(newEvent);
}

void EventQueue::PushEvent(Event&& newEvent)
{
    m_events.push_front(newEvent);
}

Event EventQueue::PopEvent()
{
    Event event(std::move(m_events.back()));
    m_events.pop_back();
    
    return event;
}

unsigned int EventQueue::GetSize() const
{
    return static_cast<unsigned int>(m_events.size());
}

void EventQueue::MakeEventUnique(EventType eventType)
{
    std::deque<Event> uniqueQueue;
    bool lock = false;

    for(const auto& currentEvent : m_events)
    {
        if(currentEvent.GetType() == eventType)
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

    m_events = std::move(uniqueQueue);
}

void EventQueue::ClearEventQueue()
{
   m_events.clear();
}