#include "Ullpch.h"
#include "Application.h"
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


using namespace Ull;

Application::Application()
{
    try
    {
        InitLog();
        Init();
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
    ULOGD("Application Terminated");
}

void Application::Run()
{
    while(m_window->IsOpen())
    {
        HandleEvents();

        Tester1();

        m_window->SwapBuffers();
    }
}

void Application::Init()
{
    //GLFW
    if (glfwInit() == -1)
        throw std::exception("Can't initialize GLFW");

    m_window = std::make_shared<Window>("Ullmanite 0.01", glm::ivec2(1280, 720));

    //Events
    m_eventQueue = std::make_unique<EventQueue>();
}

void Application::HandleEvents()
{
    m_window->PullEvents(m_eventQueue.get());
    
    std::map<Keyboard::Key, bool> updatedKeyMap;
    std::map<Mouse::Button, bool> updatedButtonMap;
    int scroll = 0;

    while(m_eventQueue->GetSize() > 0)
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

void Application::Tester1()
{
    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::SPACE))
        ULOGD("Keyboard: Space");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::ZERO))
        ULOGD("Keyboard: 0");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::ONE))
        ULOGD("Keyboard: 1");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::TWO))
        ULOGD("Keyboard: 2");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::THREE))
        ULOGD("Keyboard: 3");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::FOUR))
        ULOGD("Keyboard: 4");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::FIVE))
        ULOGD("Keyboard: 5");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::SIX))
        ULOGD("Keyboard: 6");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::SEVEN))
        ULOGD("Keyboard: 7");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::EIGHT))
        ULOGD("Keyboard: 8");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::NINE))
        ULOGD("Keyboard: 9");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::A))
        ULOGD("Keyboard: A");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::B))
        ULOGD("Keyboard: B");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::C))
        ULOGD("Keyboard: C");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::D))
        ULOGD("Keyboard: D");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::E))
        ULOGD("Keyboard: E");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F))
        ULOGD("Keyboard: F");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::G))
        ULOGD("Keyboard: G");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::H))
        ULOGD("Keyboard: H");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::I))
        ULOGD("Keyboard: I");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::J))
        ULOGD("Keyboard: J");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::K))
        ULOGD("Keyboard: K");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::M))
        ULOGD("Keyboard: M");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::N))
        ULOGD("Keyboard: N");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::O))
        ULOGD("Keyboard: O");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::P))
        ULOGD("Keyboard: P");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::Q))
        ULOGD("Keyboard: Q");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::R))
        ULOGD("Keyboard: R");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::S))
        ULOGD("Keyboard: S");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::T))
        ULOGD("Keyboard: T");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::U))
        ULOGD("Keyboard: U");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::V))
        ULOGD("Keyboard: V");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::W))
        ULOGD("Keyboard: W");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::Y))
        ULOGD("Keyboard: Y");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::Z))
        ULOGD("Keyboard: Z");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::ESCAPE))
        ULOGD("Keyboard: ESCAPE");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::ENTER))
        ULOGD("Keyboard: ENTER");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::TAB))
        ULOGD("Keyboard: TAB");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::BACKSPACE))
        ULOGD("Keyboard: BACKSPACE");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::LEFT))
        ULOGD("Keyboard: LEFT");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::RIGHT))
        ULOGD("Keyboard: RIGHT");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::UP))
        ULOGD("Keyboard: UP");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::DOWN))
        ULOGD("Keyboard: DOWN");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F1))
        ULOGD("Keyboard: F1");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F2))
        ULOGD("Keyboard: F2");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F3))
        ULOGD("Keyboard: F3");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F4))
        ULOGD("Keyboard: F4");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F5))
        ULOGD("Keyboard: F5");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F6))
        ULOGD("Keyboard: F6");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F7))
        ULOGD("Keyboard: F7");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F8))
        ULOGD("Keyboard: F8");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F9))
        ULOGD("Keyboard: F9");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F10))
        ULOGD("Keyboard: F10");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F11))
        ULOGD("Keyboard: F11");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::F12))
        ULOGD("Keyboard: F12");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::L_SHFT))
        ULOGD("Keyboard: LEFT SHIFT");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::R_SHIFT))
        ULOGD("Keyboard: RIGHT SHIFT");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::L_CONTROL))
        ULOGD("Keyboard: LEFT CONTROL");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::R_CONTROL))
        ULOGD("Keyboard: RIGHT CONTROL");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::L_ALT))
        ULOGD("Keyboard: LEFT ALT");

    if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::R_ALT))
        ULOGD("Keyboard: RIGHT ALT");

    if (Mouse::GetInstance()->IsButtonPressed(Mouse::Button::LEFT))
        ULOGD("Mouse: LEFT");

    if (Mouse::GetInstance()->IsButtonPressed(Mouse::Button::RIGHT))
        ULOGD("Mouse: RIGHT");

    if (Mouse::GetInstance()->IsButtonPressed(Mouse::Button::MIDDLE))
        ULOGD("Mouse: MIDDLE");

    /*
    auto pos = Mouse::GetInstance()->GetMousePosition();
    ULOGD("Mouse pos: " << pos.x << ", " << pos.y);

    auto scroll = Mouse::GetInstance()->GetScrollDelta();
    ULOGD("Mouse scroll: " << scroll);

    */
}