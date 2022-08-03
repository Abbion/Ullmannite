#pragma once
#include <glm/glm.hpp>
#include <glfw/glfw3.h>
#include <map>

namespace Ull
{
    class Mouse
    {
    public:
        enum class Button
        {
            LEFT = GLFW_MOUSE_BUTTON_LEFT,
            RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
            MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
        };

    public:
        static Mouse* GetInstance();

        bool IsButtonPressed(Button button) const;
        glm::ivec2 GetMousePosition() const { return m_mousePosition; }
        int GetScrollDelta() const { return m_scroll; }

        ~Mouse();
        void operator=(const Mouse&) = delete;
        Mouse(Mouse&) = delete;
        Mouse(Mouse&&) = delete;

    protected:
        Mouse();

        static Mouse* m_mouseInstance;

    private:
        std::map<Button, bool> m_buttonMap;
        glm::ivec2 m_mousePosition;
        int m_scroll;

        void UpdatePosition(const glm::ivec2& position) { m_mousePosition = position; }
        void UpdateScroll(int scroll) { m_scroll = scroll; }
        void UpdateButtonMap(const std::map<Button, bool>& updatedButtonMap);
        void InitButtonMap();

        friend class Application;
    };
};