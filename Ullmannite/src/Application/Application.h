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

		bool FailedToInitialize() const { return m_initFailed; }

		void Run();

	private:
		std::shared_ptr<UllWindow> m_window{ nullptr };
		std::unique_ptr<EventQueue> m_eventQueue{ nullptr };
		std::unique_ptr<LayerManager> m_layerManager{ nullptr };

		bool m_initFailed{ false };

		void InitApplciation();
		void HandleEvents();

		void WindowResizeHandler(const glm::uvec2& size);
	};
}