#include "Ullpch.h"
#include "Application.h"

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Rendering/Api/ShaderManager.h"

#include "Layer/Layer.h"
#include "Layer/MainLayer.h"

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
    Renderer::GetInstance()->SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    while (m_window->IsOpen())
    {
        m_window->CheckCursorInteractions();
        HandleEvents();

        if (Keyboard::GetInstance()->IsKeyPressed(Keyboard::Key::ESCAPE))
        {
            m_window->Close();
        }

        Renderer::GetInstance()->Clear(Renderer::ClearBits::COLOR);
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

    //Load Shaders
    ShaderManager::GetInstance()->LoadShader(ShaderTag::UI_SHADER, "TestVertex", "TestPixel");

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
        case EventType::WindowRestored:
            m_window->Restore();
            break;

        case EventType::WindowResize:
            WindowResizeHandler(dynamic_cast<WindowResizeEvent*>(currentEvent.get())->GetVal());
            break;

        case EventType::KeyDown:
            updatedKeyMap[dynamic_cast<KeyDownEvent*>(currentEvent.get())->GetVal()] = true;
            break;

        case EventType::KeyUp:
            updatedKeyMap[dynamic_cast<KeyUpEvent*>(currentEvent.get())->GetVal()] = false;
            break;

        case EventType::MouseDown: {
            updatedButtonMap[dynamic_cast<MouseDownEvent*>(currentEvent.get())->GetVal()] = true;
            break; }

        case EventType::MouseUp:
            updatedButtonMap[dynamic_cast<MouseDownEvent*>(currentEvent.get())->GetVal()] = false;
            break;

        case EventType::MouseMove:
            Mouse::GetInstance()->UpdatePosition(dynamic_cast<MouseMoveEvent*>(currentEvent.get())->GetVal());
            break;

        case EventType::MouseScroll:
            scroll = dynamic_cast<MouseScrollEvent*>(currentEvent.get())->GetVal();
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
    Renderer::GetInstance()->SetViewPort(glm::uvec2(0, 0), size);
}