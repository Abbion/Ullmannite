#pragma once
#include "Ullpch.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <map>

namespace Ull
{
    class Mouse
    {
    public:
        enum class Button : uint16_t
        {
            LEFT = GLFW_MOUSE_BUTTON_LEFT,
            RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
            MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
        };

    public:
        NON_COPYABLE(Mouse);

        ~Mouse();

        static Mouse& GetInstance();
        
        bool IsButtonPressed(Button button) const;
        glm::ivec2 GetMousePosition() const { return m_mousePosition; }
        glm::ivec2 GetMousePositionDelta() const { return m_mousePositionDelta; }
        int GetScrollDelta() const { return m_scroll; }

    protected:
        Mouse();

        static Mouse m_mouseInstance;

    private:
        std::map<Button, bool> m_buttonMap;
        glm::ivec2 m_mousePosition{ 0, 0 };
        glm::ivec2 m_mousePositionDelta{ 0, 0 };
        int m_scroll;

    private:
        void UpdatePosition(const glm::ivec2& position); 
        void UpdateScroll(int scroll) { m_scroll = scroll; }
        void UpdateButtonMap(const std::map<Button, bool>& updatedButtonMap);
        void InitButtonMap();

        friend class Application;
    };
};