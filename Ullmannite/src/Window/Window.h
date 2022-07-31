#pragma once
#include "Event/EventQueue.h"
#include <string>
#include <glm/glm.hpp>
#include <glfw/glfw3.h>
#include <memory>


namespace Ull
{
    class Window
    {
    public:
        Window(std::string title, glm::uvec2 size);
        ~Window();

        void SetTitle(const std::string& title);
        void SetSize(const glm::uvec2& size);
        void SetPosition(const glm::ivec2& position);

        glm::uvec2 GetPosition() const { return m_position; }
        glm::uvec2 GetSize() const { return m_size; }
        std::string GetTitle() const { return m_title; }

        bool IsOpen() const { return m_isOpen; }
        bool IsMinimized() const { return m_isMinimized; }

        void Minimize();
        void Maximize();
        void RestoreSize();
        void Close();

        void SwapBuffers();
        void PullEvents(EventQueue* eventQueue);

    private:
        glm::uvec2 m_size;
        glm::ivec2 m_position;
        std::string m_title;

        bool m_isOpen{ true };
        bool m_isMinimized{ false };

        GLFWwindow* m_window;

    private:
        void InitCallBacks();
    };
}
