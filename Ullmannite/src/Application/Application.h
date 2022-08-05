#pragma once
#include "Window/Window.h"
#include "Event/EventQueue.h"
#include <memory>

namespace Ull
{
	class Application
	{
	public:
		Application();
		~Application();

		bool FailedToInitialize() const { return m_initFailed; }

		void Run();

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<EventQueue> m_eventQueue;

		void InitApplication();
		void HandleEvents();

		bool m_initFailed{ false };
	};
}