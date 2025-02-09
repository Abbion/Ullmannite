#pragma once
#include "Window/UllWindow.h"
#include "Event/EventQueue.h"
#include "Layer/LayerManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Rendering/Api/Renderer.h"
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

	private:
		UllWindow m_window;
		EventQueue m_eventQueue;
		LayerManager m_layerManager;

		inline static Keyboard m_keyboard;
		inline static Mouse m_mouse;
		inline static Renderer m_renderer;

		bool m_initFailed{ false };

	private:
		void UpdateAndRenderLayers();
		void InitApplciation();
		void HandleEvents();
		void WindowResizeHandler(const glm::uvec2& size);
		void WindowRefreshFunction();
	};
}