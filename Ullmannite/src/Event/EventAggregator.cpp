#include "Ullpch.h"
#include "EventAggregator.h"

using namespace Ull;

void EventAggregator::Publish(const std::shared_ptr<Event>& event)
{
	for (const auto& subscribeObject : m_subscribers)
	{
		auto [id, type, fun] = subscribeObject;
		
		if (event->GetType() == type)
			fun(event);
	}
}

unsigned int EventAggregator::Sbuscribe(EventType eventType, std::function<void(const std::shared_ptr<Event>&)> fun)
{
	m_subscribers.push_back(std::make_tuple(m_nextEventID, eventType, fun));
	return m_nextEventID++;
}

void EventAggregator::Unsubscribe(unsigned int eventID)
{
	/*
	int itr = 0;

	std::find_if(m_subscribers.begin(), m_subscribers.end(), [eventID, &itr](const std::tuple<unsigned int, EventType, std::function<void>>& subscribeObject)
	{
		auto [id, type, fun] = subscribeObject;
		if (id == eventID)
			return itr++;
	});
	*/
}
