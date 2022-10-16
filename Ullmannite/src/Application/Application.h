#pragma once
#include "Window/UllWindow.h"
#include "Event/EventQueue.h"
#include "Layer/LayerManager.h"
#include <memory>

namespace Ull
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
		bool FailedToInitialize() const { return m_initFailed; }

	private:
		UllWindow m_window;
		EventQueue m_eventQueue;
		LayerManager m_layerManager;

		bool m_initFailed{ false };

	private:
		void InitApplciation();
		void HandleEvents();
		void WindowResizeHandler(const glm::uvec2& size);
	};
}