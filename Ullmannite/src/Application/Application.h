#pragma once
#include "Window/Window.h"
#include "Event/EventQueue.h"
#include "Input/Keyboard.h"
#include <memory>

namespace Ull
{
	class Application
	{
	public:
		Application();
		~Application();

		bool FailedToInitialize() { return m_initFailed; }

		void Run();

		void Tester1();

	private:
		std::shared_ptr<Window> m_window;
		std::unique_ptr<EventQueue> m_eventQueue;

		void Init();
		void HandleEvents();
		bool m_initFailed{ false };
	};
}