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

#include "Resources/ResourceManager.h"

#include "Rendering/IconsCode/IconCodes.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Output/Image2DWriter.h"

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
    ULOGD("Application terminated");
}

void Application::Run()
{    
    while (m_window.IsOpen())
    {        
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

    m_window.Create("Ullmanite 0.7v", glm::uvec2(1280, 720));
    m_window.SetEventQueueDataPointer(&m_eventQueue);
    m_window.SetRefreshFunction([this]() { WindowRefreshFunction(); });

    //Renderer
    glfwMakeContextCurrent(m_window.GetWindowContext());

    Renderer::GetInstance().Init();
    Renderer::GetInstance().SetViewPort(glm::uvec2(0, 0), m_window.GetSize());

    //Load Shaders
    auto& shaderManager = Renderer::GetInstance().GetShaderManager();

    shaderManager.LoadShader(ShaderTag::UI_BASIC_COLOR, "UiBasicVS", "UiBasicColorPS");
    shaderManager.LoadShader(ShaderTag::FRAME_DISPLAY_SHADER, "DisplayFrameVS", "DisplayFramePS");
    shaderManager.LoadShader(ShaderTag::MARKER, "MarkerVS", "MarkerPS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_MESH_GENERATOR, "CubeMarchCS");
    shaderManager.LoadShader(ShaderTag::TRANSFER_FUNCTION_GENERATOR, "TransferFunctionGeneratorCS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_VERTEX_COUNTER, "CubeMarchVertexCounterCS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_VERTEX_RENDERER, "CubeMarchVS", "CubeMarchPS", "CubeMarchGS");
    shaderManager.LoadShader(ShaderTag::UI_GRADIENT_SHADER, "UiGradientVS", "UiGradientPS");
    shaderManager.LoadShader(ShaderTag::SIGNED_DISTANCE_FIELD_FOR_2D_TEXTURES, "SignedDistanceField2DCS");
    shaderManager.LoadShader(ShaderTag::CHANGE_VALUE_IF_GREATHER_THAN_UIIMAGE_2D, "ChangeValueInUIImage2DIfGreaterThan");

    //Resources
    auto& fontManager = ResourceManager::GetInstance().GetFontMnager();
    fontManager.InitLoader();
    fontManager.LoadFont("arial.ttf", FontTag::UI_FONT);
    fontManager.ReleaseLoader();

    //Layers
    auto mainLayer = std::make_shared<MainLayer>(m_window.GetSize());
    mainLayer->SetWindow(NotOwner<UllWindow>(&m_window));
    m_layerManager.PushLayer(mainLayer);

    //First resizeEvent to inform components of the initial window size
    const auto windowSize = m_window.GetSize();
    m_eventQueue.PushEvent(std::make_shared<WindowResizeEvent>(EventType::WindowResize, glm::uvec2(windowSize.x, windowSize.y)));
}

void Application::HandleEvents()
{
    m_eventQueue.MakeEventUnique(EventType::WindowResize);

    Keyboard::KeyState keyState{};
    Mouse::ButtonState buttonState{};
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
            keyState.key = static_cast<KeyDownEvent*>(currentEvent.get())->GetVal();
            keyState.state = true;
            break;

        case EventType::KeyUp:
            keyState.key = static_cast<KeyUpEvent*>(currentEvent.get())->GetVal();
            keyState.state = false;
            break;

        case EventType::MouseDown:
            buttonState.button = static_cast<MouseDownEvent*>(currentEvent.get())->GetVal();
            buttonState.state = true;
            break;

        case EventType::MouseUp:
            buttonState.button = static_cast<MouseDownEvent*>(currentEvent.get())->GetVal();
            buttonState.state = false;
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

    Keyboard::GetInstance().UpdateKeyMap(keyState);
    Mouse::GetInstance().UpdateButtonMap(buttonState);
    Mouse::GetInstance().UpdateScroll(scroll);
}

void Application::WindowResizeHandler(const glm::uvec2& size)
{
    Renderer::GetInstance().SetViewPort(glm::uvec2(0, 0), size);
}

void Application::WindowRefreshFunction()
{
    HandleEvents();

    m_layerManager.GetTopLayer()->Update();
    if (!m_window.IsMinimized())
    {
        m_layerManager.GetTopLayer()->Render();
        m_window.SwapBuffers();
    }
}
