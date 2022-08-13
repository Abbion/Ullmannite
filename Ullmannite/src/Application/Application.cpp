#include "Ullpch.h"
#include "Application.h"

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Layer/Layer.h"

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
    delete Renderer::GetInstance();
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
    
    //Renderer
    Renderer::GetInstance()->SetApi(Renderer::API::OPEN_GL);

    //Layers
    m_layerManager = std::make_unique<LayerManager>();
    m_layerManager->PushLayer(std::make_shared<Layer>("Test layer 1"));
    m_layerManager->PushLayer(std::make_shared<Layer>("Test layer 2"));
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

        case EventType::KeyDown:
            updatedKeyMap[static_cast<KeyDownEvent*>(currentEvent.get())->GetVal()] = true;
        break;

        case EventType::KeyUp:
            updatedKeyMap[static_cast<KeyUpEvent*>(currentEvent.get())->GetVal()] = false;
        break;

        case EventType::MouseDown:
            updatedButtonMap[static_cast<MouseDownEvent*>(currentEvent.get())->GetVal()] = true;
        break;

        case EventType::MouseUp:
            updatedButtonMap[static_cast<MouseDownEvent*>(currentEvent.get())->GetVal()] = false;
        break;

        case EventType::MouseMove:
            Mouse::GetInstance()->UpdatePosition(static_cast<MouseMoveEvent*>(currentEvent.get())->GetVal());
        break;

        case EventType::MouseScroll:
            scroll = static_cast<MouseScrollEvent*>(currentEvent.get())->GetVal();
        break;
        
        default:
        break;
        }

        if(!currentEvent->IsHandeled())
            m_layerManager->HandleEvent(currentEvent.get());
    }

    Keyboard::GetInstance()->UpdateKeyMap(updatedKeyMap);
    Mouse::GetInstance()->UpdateButtonMap(updatedButtonMap);
    Mouse::GetInstance()->UpdateScroll(scroll);
}