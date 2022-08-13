#pragma once
#include "Window/Window.h"
#include "Event/EventQueue.h"
#include "Layer/LayerManager.h"
#include "Rendering/Api/Renderer.h"
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
		std::unique_ptr<LayerManager> m_layerManager;
		std::unique_ptr<Renderer> m_renderer;

		bool m_initFailed{ false };

		void InitApplication();
		void HandleEvents();
	};
}