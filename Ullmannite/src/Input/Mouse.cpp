#include "Ullpch.h"
#include "Mouse.h"
#include "Logger/Logger.h"
#include "Window/UllWindow.h"

using namespace Ull;

Mouse::Mouse() : m_mousePosition(0, 0), m_scroll(0)
{
    InitButtonMap();
}

Mouse::~Mouse()
{
    m_buttonMap.clear();
}

void Mouse::SetCursorMode(const Mode mode)
{
    m_cursorMode = mode;
    m_changeCursor = true;
}

bool Mouse::IsButtonPressed(Button button) const
{
    return m_buttonMap.at(button);
}

void Mouse::UpdateButtonMap(const ButtonState buttonState)
{
    if(buttonState.button != Button::NONE)
        m_buttonMap[buttonState.button] = buttonState.state;
}

void Mouse::InitButtonMap()
{
    m_buttonMap[Button::LEFT] = false;
    m_buttonMap[Button::RIGHT] = false;
    m_buttonMap[Button::MIDDLE] = false;
}

void Mouse::UpdatePosition(const glm::ivec2& position)
{
    static bool firstEnter = true;
    if(firstEnter)
    {
        m_mousePosition = position;
        firstEnter = false;
    }

    m_mousePositionDelta =  position - m_mousePosition;
    m_mousePosition = position;
}

void Mouse::UpdateCursorMode(UllWindow& window)
{
    if (!m_changeCursor)
        return;

    glfwDestroyCursor(m_cursor);

    int glfwCursor = GLFW_ARROW_CURSOR;

    switch (m_cursorMode)
    {
    case Mode::IBEAM:
        glfwCursor = GLFW_IBEAM_CURSOR;
        break;

    default:
        glfwCursor = GLFW_ARROW_CURSOR;
        break;
    }

    m_cursor = glfwCreateStandardCursor(glfwCursor);
    glfwSetCursor(window.GetWindowContext(), m_cursor);
    m_changeCursor = false;
}