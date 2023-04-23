#pragma once
#include "Event/EventQueue.h"
#include "Event/EventHandler.h"

#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include <chrono>

namespace Ull
{
    class UllWindow : public EventHandler
    {
    public:
        UllWindow();
        ~UllWindow();

        void Create(std::string title, glm::uvec2 size);
        void SetTitle(const std::string& title);
        void SetSize(glm::ivec2 size);
        void SetPosition(glm::ivec2 position);
        void SetEventQueueDataPointer(EventQueue* eventQueue);
        void SetRefreshFunction(std::function<void()> refreshFunction);
        void SetDragArea(const glm::uvec2 position, const glm::uvec2 size);

        void HandleEvent(Event* event) override;

        const glm::ivec2 GetPosition() const;
        const glm::ivec2 GetSize() const;
        const std::string GetTitle() const { return m_title; }

        GLFWwindow* GetWindowContext() const { return m_window; }

        const bool IsOpen() const { return m_isOpen; }
        const bool IsMinimized() const { return m_isMinimized; }
        const bool IsMaximized() const { return m_isMaximized; }
        const bool IsCursorLocked() const { return m_cursorLocked; }

        void Close();
        void Maximize();
        void Minimize();
        void Restore();

        void SwapBuffers();
        void PullEvents();

    private:
        struct WindowPointerDataStruct {
            EventQueue* eventQueue;
            std::function<void()> refreshFunction;
        };

        WindowPointerDataStruct m_windowPointerDataStruct;

    private:
        std::string m_title;
        bool m_isOpen{ true };
        bool m_isMinimized{ false };
        bool m_isMaximized{ false };
        bool m_cursorLocked{ false };
        int m_scaleCounter{ 0 };

        GLFWwindow* m_window{ nullptr };

        std::chrono::time_point<std::chrono::steady_clock> m_lastRefresh;

        void SwitchHiddenCursor();

        void InitCallBacks();
    };
}
