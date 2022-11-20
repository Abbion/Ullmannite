#pragma once
#include "Event.h"
#include <vector>
#include <functional>

namespace Ull
{
	class EventAggregator
	{
	public:
		static void Publish(const std::shared_ptr<Event>& event);
		static unsigned int Sbuscribe(EventType eventType, std::function<void(const std::shared_ptr<Event>&)> fun);
		static void Unsubscribe(unsigned int eventID);

	private:
		inline static std::vector<std::tuple<unsigned int, EventType, std::function<void(const std::shared_ptr<Event>&)>>> m_subscribers;
		inline static unsigned int m_nextEventID{ 0 };
	};
}