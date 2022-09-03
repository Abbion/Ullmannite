#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include "Event/EventQueue.h"
#include <chrono>

namespace Ull
{
    enum class ResizeBorder //TODO: change to resize frame
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

    class Window
    {
    public:
        Window(std::string title, glm::uvec2 size);
        ~Window();

        void SetTitle(const std::string& title);
        void SetSize(glm::ivec2 size);
        void SetPosition(glm::ivec2 position);
        void SetEventQueueDataPointer(EventQueue* eventQueue);

        glm::ivec2 GetPosition() const;
        glm::ivec2 GetSize() const;
        std::string GetTitle() const { return m_title; }


        GLFWwindow* GetRenderContext() { return m_renderWindow; }
        GLFWwindow* GetEventContext()  { return m_eventContext; }

        bool IsOpen() const { return m_isOpen; }
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
        GLFWwindow* m_renderWindow{ nullptr };
        GLFWwindow* m_eventContext{ nullptr };
        GLFWcursor* m_cursor{ nullptr };

        uint8_t m_resizeBorderSize{ 6 };

        glm::dvec2 m_startGrabPosition;

        bool m_isDragged{ false };
        bool m_isResized{ false };
        glm::ivec2 m_windowPositionWithMinSize;

        ResizeBorder m_resizeBorder{ ResizeBorder::NONE };

        std::chrono::time_point<std::chrono::steady_clock> m_lastRefresh;
        bool m_intervalRestored{ false };

        void CheckResizeBorder();
        void ResizeByCursor();
        void MovedByCursor();

        glm::ivec2 GetCursorScreenPosition();

        void InitCallBacks();
    };
}
