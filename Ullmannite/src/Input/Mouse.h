#pragma once
#include "Ullpch.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <map>

namespace Ull
{
    class UllWindow;

    class Mouse
    {
    public:
        enum class Button : uint16_t
        {
            NONE = 404u,
            LEFT = GLFW_MOUSE_BUTTON_LEFT,
            RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
            MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
        };

        enum class Mode : uint16_t
        {
            POINTER,
            IBEAM
        };

        struct ButtonState 
        {
            Button button = Button::NONE;
            bool state = false;
        };

    public:
        NON_COPYABLE(Mouse);
        ~Mouse();
        
        void SetCursorMode(const Mode mode);

        bool IsButtonPressed(Button button) const;
        glm::ivec2 GetMousePosition() const { return m_mousePosition; }
        glm::ivec2 GetMousePositionDelta() const { return m_mousePositionDelta; }
        int GetScrollDelta() const { return m_scroll; }

    protected:
        void UpdateCursorMode(UllWindow& window);

    private:
        Mouse();
        void UpdatePosition(const glm::ivec2& position); 
        void UpdateScroll(int scroll) { m_scroll = scroll; }
        void UpdateButtonMap(const ButtonState buttonState);
        void InitButtonMap();

        bool m_changeCursor{ true };
        Mode m_cursorMode{ Mode::POINTER };
        GLFWcursor* m_cursor{ nullptr };

        std::map<Button, bool> m_buttonMap;
        glm::ivec2 m_mousePosition{ 0, 0 };
        glm::ivec2 m_mousePositionDelta{ 0, 0 };
        int m_scroll;

        friend class Application;
    };
};