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
    enum class ResizeFrame
    {
        NONE,
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };

    class UllWindow : public EventHandler
    {
    public:
        UllWindow(std::string title, glm::uvec2 size);
        ~UllWindow();

        void SetTitle(const std::string& title);
        void SetSize(glm::ivec2 size);
        void SetPosition(glm::ivec2 position);
        void SetEventQueueDataPointer(EventQueue* eventQueue);
        void EnableDrag(bool enable) { m_dragEnabled = enable; }

        void HandleEvent(Event* event) override;

        glm::ivec2 GetPosition() const;
        glm::ivec2 GetSize() const;
        std::string GetTitle() const { return m_title; }

        GLFWwindow* GetWindowContext() const { return m_window; }

        bool IsOpen() const { return m_isOpen; }
        bool IsMinimized() const { return m_isMinimized; }
        bool IsMaximized() const { return m_isMaximized; }
        bool IsCursorLocked() const { return m_cursorLocked; }
        bool IsDragged() const { return m_isDragged; }

        void Close();
        void Maximize();
        void Minimize();
        void Restore();

        void CheckCursorInteractions();

        void SwapBuffers();
        void PullEvents();

    private:
        std::string m_title;
        bool m_isOpen{ true };
        bool m_isMinimized{ false };
        bool m_isMaximized{ false };
        bool m_cursorLocked{ false };

        GLFWwindow* m_window{ nullptr };

        GLFWcursor* m_cursor{ nullptr };
        int m_currentCursorShape{ GLFW_ARROW_CURSOR };

        uint8_t m_resizeBorderSize{ 6 };
        glm::ivec2 m_startGrabPosition;

        bool m_dragEnabled{ false };
        bool m_isDragged{ false };
        bool m_isResized{ false };
        glm::ivec2 m_windowPositionWithMinSize;

        ResizeFrame m_resizeBorder{ ResizeFrame::NONE };

        std::chrono::time_point<std::chrono::steady_clock> m_lastRefresh;
        bool m_intervalRestored{ false };

        void CheckResizeBorder();
        void ResizeByCursor();
        void MovedByCursor();
        void ChangeCursorShape(int shape);
        void SwitchHiddenCursor();

        glm::ivec2 GetCursorScreenPosition();

        void InitCallBacks();
    };
}
