#pragma once
#include "Window/Window.h"
#include "Event/EventQueue.h"
#include "Layer/LayerManager.h"

#include <memory>
#include <mutex>
#include <condition_variable>

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
		std::unique_ptr<Window> m_window{ nullptr };
		std::unique_ptr<EventQueue> m_eventQueue{ nullptr };
		std::unique_ptr<LayerManager> m_layerManager{ nullptr };

		bool m_initFailed{ false };
		bool m_contextCreationFailed{ false };

		void InitApplciation();
		void InitWindow();
		bool RenderContextCreated();

		std::thread m_eventPullThread;
		std::mutex m_initMutex;
		std::condition_variable m_initCV;

		void HandleEvents();

		void WindowResizeHandler(const glm::uvec2& size);
	};
}