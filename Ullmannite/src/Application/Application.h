#pragma once
#include "Window/UllWindow.h"
#include "Event/EventQueue.h"
#include "Layer/LayerManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Rendering/Api/Renderer.h"
#include "Resources/ResourceManager.h"
#include <thread>
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

		static Keyboard& GetKeyboard() { return m_keyboard; }
		static Mouse& GetMouse() { return m_mouse; }
		static Renderer& GetRenderer() { return m_renderer; }
		static EventQueue& GetEventQueue() { return m_eventQueue; }
		static ResourceManager& GetResourceManager() { return m_resourceManager; }
		static UllWindow& GetWindow() { return m_window; }

	private:
		inline static UllWindow m_window;
		LayerManager m_layerManager;
		std::thread m_loaderThread;

		inline static EventQueue m_eventQueue;
		inline static Keyboard m_keyboard;
		inline static Mouse m_mouse;
		inline static Renderer m_renderer;
		inline static ResourceManager m_resourceManager;

		bool m_initFailed{ false };

	private:
		void UpdateAndRenderLayers();
		void InitApplciation();
		void HandleEvents();
		void WindowResizeHandler(const glm::uvec2& size);
		void WindowRefreshFunction();
	};
}