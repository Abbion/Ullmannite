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

#include "Rendering/IconsCode/IconCodes.h"

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
    while (m_window.IsOpen())
    {
        m_window.CheckCursorInteractions();
        
        if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::ESCAPE))
        {
            m_window.Close();
        }

        m_window.PullEvents();
        HandleEvents();
        
        m_layerManager.GetTopLayer()->Update();
        
        if (!m_window.IsMinimized())
        {
            m_layerManager.GetTopLayer()->Render();
            m_window.SwapBuffers();
        }
    }
}

void Application::InitApplciation()
{
    Ull::Renderer::GetInstance().SetApi(Ull::Renderer::API::OPEN_GL);

    //Window
    if (glfwInit() == -1)
        throw InitializationException("Can't initialize GLFW");

    m_window.Create("Ullmanite 0.3v", glm::uvec2(1280, 720));
    m_window.SetEventQueueDataPointer(&m_eventQueue);

    //Renderer
    glfwMakeContextCurrent(m_window.GetWindowContext());

    Renderer::GetInstance().Init();
    Renderer::GetInstance().SetViewPort(glm::uvec2(0, 0), m_window.GetSize());

    //ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 140";
    ImGui_ImplGlfw_InitForOpenGL(m_window.GetWindowContext(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans-Regular-Short.ttf", 13.0f);
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 14.0f;
    static const ImWchar icon_ranges[] = { ICON_START, ICON_END, 0 };
    io.Fonts->AddFontFromFileTTF("Assets/Fonts/UllIcon.ttf", 22.0f, &config, icon_ranges);

    //Load Shaders
    ShaderManager::GetInstance().LoadShader(ShaderTag::UI_SHADER, "TestVertex", "TestPixel");
    ShaderManager::GetInstance().LoadShader(ShaderTag::FRAME_DISPLAY_SHADER, "DisplayFrameVS", "DisplayFramePS");
    ShaderManager::GetInstance().LoadShader(ShaderTag::MARKER, "MarkerVS", "MarkerPS");
    ShaderManager::GetInstance().LoadShader(ShaderTag::CUBE_MARCH_MESH_GENERATOR, "CubeMarchCS");
    ShaderManager::GetInstance().LoadShader(ShaderTag::TRANSFER_FUNCTION_GENERATOR, "TransferFunctionGeneratorCS");
    ShaderManager::GetInstance().LoadShader(ShaderTag::CUBE_MARCH_VERTEX_COUNTER, "CubeMarchVertexCounterCS");
    ShaderManager::GetInstance().LoadShader(ShaderTag::CUBE_MARCH_VERTEX_RENDERER, "CubeMarchVS", "CubeMarchPS", "CubeMarchGS");
    
    //Layers
    auto mainLayer = std::make_shared<MainLayer>(m_window.GetSize());
    mainLayer->SetWindow(NotOwner<UllWindow>(&m_window));
    m_layerManager.PushLayer(mainLayer);
}

void Application::HandleEvents()
{
    m_eventQueue.MakeEventUnique(EventType::WindowResize);

    std::map<Keyboard::Key, bool> updatedKeyMap;
    std::map<Mouse::Button, bool> updatedButtonMap;
    int scroll = 0;

    while (m_eventQueue.HasPenddingEvents())
    {
        auto currentEvent = m_eventQueue.PopEvent();

        switch (currentEvent->GetType())
        {
        case EventType::WindowClosed:
            m_window.Close();
            break;
        case EventType::WindowRestored:
            m_window.Restore();
            break;

        case EventType::WindowResize:
            if (m_window.IsMinimized())
                currentEvent->MarkHandeled(true);
            else
                WindowResizeHandler(static_cast<WindowResizeEvent*>(currentEvent.get())->GetVal());
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
            Mouse::GetInstance().UpdatePosition(static_cast<MouseMoveEvent*>(currentEvent.get())->GetVal());
            break;

        case EventType::MouseScroll:
            scroll = static_cast<MouseScrollEvent*>(currentEvent.get())->GetVal();
            break;

        default:
            break;
        }

         m_window.HandleEvent(currentEvent.get());
         m_layerManager.HandleEvent(currentEvent.get());
    }

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