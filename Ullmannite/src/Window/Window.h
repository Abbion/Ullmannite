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
        void SetSize(glm::uvec2 size);
        void SetPosition(glm::ivec2 position);

        glm::ivec2 GetPosition() const;
        glm::ivec2 GetSize() const;
        std::string GetTitle() const { return m_title; }

        bool IsOpen() const { return m_isOpen; }
        bool IsMinimized() const { return m_isMinimized; }

        void Minimize();
        void Maximize();
        void Close();

        void SwapBuffers();
        void PullEvents(EventQueue* eventQueue);

    private:
        std::string m_title;

        bool m_isOpen{ true };
        bool m_isMinimized{ false };

        GLFWwindow* m_window;

    private:
        void InitCallBacks();
    };
}
