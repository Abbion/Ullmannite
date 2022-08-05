#include "Ullpch.h"
#include "Application.h"

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

using namespace Ull;

Application::Application()
{
    try
    {
        InitLog();
        InitApplication();
    }
    catch (const std::exception& e)
    {
        ULOGF(e.what());
        m_initFailed = true;
    }
}

Application::~Application()
{
    delete Keyboard::GetInstance();
    delete Mouse::GetInstance();
    ULOGD("Application terminated");
}

void Application::Run()
{
    while(m_window->IsOpen())
    {
        HandleEvents();

        m_window->SwapBuffers();
    }
}

void Application::InitApplication()
{
    //GLFW
    if (glfwInit() == -1)
        throw InitializationException("Can't initialize GLFW");

    m_window = std::make_unique<Window>("Ullmanite 0.02", glm::ivec2(1280, 720));

    //Events
    m_eventQueue = std::make_unique<EventQueue>();
}

void Application::HandleEvents()
{
    m_window->PullEvents(m_eventQueue.get());
    
    std::map<Keyboard::Key, bool> updatedKeyMap;
    std::map<Mouse::Button, bool> updatedButtonMap;
    int scroll = 0;

    while(m_eventQueue->HasPenddingEvents())
    {
        auto currentEvent = m_eventQueue->PopEvent();
        
        switch (currentEvent->GetType())
        {
        case EventType::WindowClosed:
            m_window->Close();
            break;

        case EventType::WindowResize:
        {
            auto size = static_cast<WindowResizeEvent*>(currentEvent.get())->GetSize();
            ULOGD("Resize: " << size.x << ", " << size.y);
        }
            break;

        case EventType::WindowLostFocus:
            ULOGD("LostFocus");
            break;

        case EventType::WindowGainedFocus:
            ULOGD("GainedFocus");
        break;

        case EventType::WindowMaximized:
            ULOGD("Maximized");
        break;

        case EventType::WindowMinimized:
            ULOGD("Minimized");
        break;

        case EventType::WindowRestored:
            ULOGD("Restored");
        break;

        case EventType::WindowMove:
        {
            auto mv = static_cast<WindowMoveEvent*>(currentEvent.get())->GetPosition();
            ULOGD("Move: " << mv.x << ", " << mv.y);
        }
            break;

        case EventType::KeyDown:
            updatedKeyMap[static_cast<KeyDownEvent*>(currentEvent.get())->GetKey()] = true;
        break;

        case EventType::KeyUp:
            updatedKeyMap[static_cast<KeyUpEvent*>(currentEvent.get())->GetKey()] = false;
        break;

        case EventType::MouseDown:
            updatedButtonMap[static_cast<MouseDownEvent*>(currentEvent.get())->GetButton()] = true;
        break;

        case EventType::MouseUp:
            updatedButtonMap[static_cast<MouseDownEvent*>(currentEvent.get())->GetButton()] = false;
        break;

        case EventType::MouseMove:
            Mouse::GetInstance()->UpdatePosition(static_cast<MouseMoveEvent*>(currentEvent.get())->GetPosition());
        break;

        case EventType::MouseScroll:
            scroll = static_cast<MouseScrollEvent*>(currentEvent.get())->GetScroll();
        break;

        default:
            break;
        }
    }

    Keyboard::GetInstance()->UpdateKeyMap(updatedKeyMap);
    Mouse::GetInstance()->UpdateButtonMap(updatedButtonMap);
    Mouse::GetInstance()->UpdateScroll(scroll);
}