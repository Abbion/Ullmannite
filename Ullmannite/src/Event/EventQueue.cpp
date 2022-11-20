#include <Ullpch.h>
#include "EventQueue.h"
#include "EventAggregator.h"

#include "Logger/Logger.h"

using namespace Ull;

EventQueue::EventQueue()
{
    EventAggregator::Sbuscribe(EventType::FileLoaded, [this](const std::shared_ptr<Event>& event) {
        PushEvent(event);
    });
    
    EventAggregator::Sbuscribe(EventType::ExaminationThresholdChanged, [this](const std::shared_ptr<Event>& event) {
        PushEvent(event);
    });
}

EventQueue::~EventQueue()
{
    ClearEventQueue();
    ULOGD("EventQueue terminated");
}

void EventQueue::PushEvent(const std::shared_ptr<Event>& newEvent)
{
    m_events.push_back(newEvent);
}

std::shared_ptr<Event> EventQueue::PopEvent()
{
    auto lastEvent = m_events.front();
    m_events.pop_front();
    return lastEvent;
}

void EventQueue::CheckPublishedEvents()
{
}

bool EventQueue::HasPenddingEvents() const
{
    return m_events.size() > 0u;
}

unsigned int EventQueue::GetSize() const
{
    return (unsigned int)m_events.size();
}

void EventQueue::MakeEventUnique(EventType eventType)
{
    std::deque<std::shared_ptr<Event>> uniqueQueue;
    bool lock = false;

    for(std::deque<std::shared_ptr<Event>>::reverse_iterator itr = m_events.rbegin(); itr != m_events.rend(); ++itr)
    {
        if((*itr)->GetType() == eventType)
        {
            if(!lock)
            {
                uniqueQueue.push_back(*itr);
                lock = true;
            }
        }
        else
            uniqueQueue.push_back(*itr);
    }

    ClearEventQueue();
    m_events = uniqueQueue;
}

void EventQueue::ClearEventQueue()
{
   m_events.clear();
}