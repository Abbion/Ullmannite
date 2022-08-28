#include "Ullpch.h"
#include "Application.h"

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Layer/Layer.h"
#include "Layer/MainLayer.h"

#include "Rendering/Converters/CoordinateConverter.h"

#include "glad/glad.h"

#include <thread>
#include <chrono>
#include <functional>

using namespace Ull;

Application::Application()
{
    try
    {
        InitLog();
        InitApplciation();
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
    glClearColor(0.9f, 0.2f, 0.2f, 1.0f);

    while (m_window->IsOpen())
    {
        m_window->CheckCursorInteractions();

        auto s = m_window->GetSize();
        int speed = 20;
        if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::W))
        {
            m_window->SetSize(glm::uvec2(s.x + speed, s.y + speed));
        }
        else if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::S))
        {
            m_window->SetSize(glm::uvec2(s.x - speed + 2, s.y - speed + 2));
        }

        HandleEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        m_layerManager->GetTopLayer()->Render();

        m_window->SwapBuffers();
    }

    if(m_eventPullThread.joinable())
        m_eventPullThread.join();
}

void Application::InitApplciation()
{
    Renderer::GetInstance()->SetApi(Renderer::API::OPEN_GL);

    //Window
    if (glfwInit() == -1)
        throw InitializationException("Can't initialize GLFW");

    //Thread that is creating the acctual window and handles event pulling form the system
    m_eventPullThread = std::thread(&Application::InitWindow, this);

    //Wait for the event pull thread to finish the window creation
    std::unique_lock<std::mutex> mlock(m_initMutex);
    m_initCV.wait(mlock, std::bind(&Application::RenderContextCreated, this));

    //Renderer
    glfwMakeContextCurrent(m_window->GetRenderContext());

    Renderer::GetInstance()->Init();
    Renderer::GetInstance()->SetViewPort(glm::uvec2(0, 0), m_window->GetSize());
    
    CoordinateConverter::GetInstance()->SetWindowSize(m_window->GetSize());

    //Layers
    m_layerManager = std::make_unique<LayerManager>();
    m_layerManager->PushLayer(std::make_shared<MainLayer>(m_window->GetSize()));
    
}

void Application::InitWindow()
{
    std::unique_lock<std::mutex> lock(m_initMutex);

    try
    {
        m_window = std::make_unique<Window>("Ullmanite 0.02", glm::ivec2(1280, 720));

        m_eventQueue = std::make_unique<EventQueue>();
        m_window->SetEventQueueDataPointer(m_eventQueue.get());
    }
    catch (const std::exception& e)
    {
        UASSERT(false, e.what());
        m_ContextCreationFailed = true;
    }

    lock.unlock();
    m_initCV.notify_one();

    if (m_ContextCreationFailed)
        return;

     while (m_window->IsOpen())
        m_window->PullEvents();
}

bool Application::RenderContextCreated()
{
    if (m_ContextCreationFailed)
    {
        if(m_eventPullThread.joinable())
            m_eventPullThread.join();
        throw InitializationException("Context creation error");
    }

    return (m_window != nullptr && m_window->GetRenderContext() != nullptr);
}

void Application::HandleEvents()
{
    m_eventQueue->LockAccess();

    m_eventQueue->MakeEventUnique(EventType::WindowResize);

    std::map<Keyboard::Key, bool> updatedKeyMap;
    std::map<Mouse::Button, bool> updatedButtonMap;
    int scroll = 0;

    while (m_eventQueue->HasPenddingEvents())
    {
        auto currentEvent = m_eventQueue->PopEvent();

        switch (currentEvent->GetType())
        {
        case EventType::WindowClosed:
            m_window->Close();
            break;

        case EventType::WindowResize:
            WindowResizeHandler(static_cast<WindowResizeEvent*>(currentEvent.get())->GetVal());
            break;

        case EventType::KeyDown:
            updatedKeyMap[static_cast<KeyDownEvent*>(currentEvent.get())->GetVal()] = true;
            break;

        case EventType::KeyUp:
            updatedKeyMap[static_cast<KeyUpEvent*>(currentEvent.get())->GetVal()] = false;
            break;

        case EventType::MouseDown: {
            updatedButtonMap[static_cast<MouseDownEvent*>(currentEvent.get())->GetVal()] = true;
            break; }

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

        if (!currentEvent->IsHandeled())
            m_layerManager->HandleEvent(currentEvent.get());
    }
    m_eventQueue->UnlockAccess();

    Keyboard::GetInstance()->UpdateKeyMap(updatedKeyMap);
    Mouse::GetInstance()->UpdateButtonMap(updatedButtonMap);
    Mouse::GetInstance()->UpdateScroll(scroll);
}

void Application::WindowResizeHandler(const glm::uvec2& size)
{
    CoordinateConverter::GetInstance()->SetWindowSize(size);
    Renderer::GetInstance()->SetViewPort(glm::uvec2(0, 0), size);
}