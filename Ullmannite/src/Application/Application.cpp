#include "Ullpch.h"
#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Rendering/Api/Renderer.h"
#include "Rendering/Api/ShaderManager.h"

#include "Layer/Layer.h"
#include "Layer/MainLayer.h"

#include <thread>
#include <chrono>

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ULOGD("Application terminated");
}

void Application::Run()
{    
    while (m_window->IsOpen())
    {
        m_window->CheckCursorInteractions();
        
        HandleEvents();

        if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::ESCAPE))
        {
            m_window->Close();
        }
        
        m_layerManager->GetTopLayer()->Render();
        
        m_window->SwapBuffers();
    }

    if(m_eventPullThread.joinable())
        m_eventPullThread.join();
}

void Application::InitApplciation()
{
    Renderer::GetInstance().SetApi(Renderer::API::OPEN_GL);

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

    Renderer::GetInstance().Init();
    Renderer::GetInstance().SetViewPort(glm::uvec2(0, 0), m_window->GetSize());

    //ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 140";
    ImGui_ImplGlfw_InitForOpenGL(m_window->GetRenderContext(), false);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Load Shaders
    ShaderManager::GetInstance().LoadShader(ShaderTag::UI_SHADER, "TestVertex", "TestPixel");
    ShaderManager::GetInstance().LoadShader(ShaderTag::FRAME_DISPLAY_SHADER, "DisplayFrameVS", "DisplayFramePS");

    //Layers
    m_layerManager = std::make_unique<LayerManager>();
    m_layerManager->PushLayer(std::make_shared<MainLayer>(m_window->GetSize()));
}

void Application::InitWindow()
{
    std::unique_lock<std::mutex> lock(m_initMutex);

    try
    {
        m_window = std::make_shared<UllWindow>("Ullmanite 0.02", glm::ivec2(1280, 720));

        m_eventQueue = std::make_unique<EventQueue>();
        m_window->SetEventQueueDataPointer(m_eventQueue.get());
    }
    catch (const std::exception& e)
    {
        UASSERT(false, e.what());
        m_contextCreationFailed = true;
    }

    lock.unlock();
    m_initCV.notify_one();

    if (m_contextCreationFailed)
        return;

    while (m_window->IsOpen())
        m_window->PullEvents();
}

bool Application::RenderContextCreated()
{
    if (m_contextCreationFailed)
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
        {
            Mouse::GetInstance().UpdatePosition(static_cast<MouseMoveEvent*>(currentEvent.get())->GetVal());
            
            auto mousePos = Mouse::GetInstance().GetMousePosition();

            ImGuiIO& io = ImGui::GetIO();
            io.AddMousePosEvent((float)mousePos.x, (float)mousePos.y);
        }
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

    Keyboard::GetInstance().UpdateKeyMap(updatedKeyMap);
    Mouse::GetInstance().UpdateButtonMap(updatedButtonMap);
    Mouse::GetInstance().UpdateScroll(scroll);

    updatedKeyMap.clear();
    updatedButtonMap.clear();
}

void Application::WindowResizeHandler(const glm::uvec2& size)
{
    Renderer::GetInstance().SetViewPort(glm::uvec2(0, 0), size);
}