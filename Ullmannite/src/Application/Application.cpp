#include "Ullpch.h"
#include "Application.h"
#include "Logger/Logger.h"
#include "Event/Event.h"

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
    ULOGD("Application Terminated");
}

void Application::Run()
{
    while(m_window->IsOpen())
    {
        HandleEvents();
        


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

    while(m_eventQueue->GetSize() > 0)
    {
        auto event = m_eventQueue->PopEvent();
        
        switch (event.GetType())
        {
        case EventType::WindowClosed:
            m_window->Close();
            break;
        default:
            break;
        }
    }
}