#include "Ullpch.h"
#include "Mouse.h"
#include "Logger/Logger.h"

using namespace Ull;

Mouse* Mouse::m_mouseInstance = {};

Mouse* Mouse::GetInstance()
{
    if (m_mouseInstance == nullptr)
        m_mouseInstance = new Mouse();

    return m_mouseInstance;
}

Mouse::Mouse() : m_mousePosition(0, 0), m_scroll(0)
{
    InitButtonMap();
}

Mouse::~Mouse()
{
    m_buttonMap.clear();
    ULOGD("Mouse terminated");
}

bool Mouse::IsButtonPressed(Button button) const
{
    return m_buttonMap.at(button);
}

void Mouse::UpdateButtonMap(const std::map<Button, bool>& updatedButtonMap)
{
    for (const auto& button : updatedButtonMap)
        m_buttonMap[button.first] = button.second;
}

void Mouse::InitButtonMap()
{
    m_buttonMap[Button::LEFT] = false;
    m_buttonMap[Button::RIGHT] = false;
    m_buttonMap[Button::MIDDLE] = false;
}