#include "Ullpch.h"
#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Input/Mouse.h"

#include "Rendering/Api/ShaderManager.h"
#include "Rendering/TriangulationTable/TriangulationTable.h"

#include "Layer/Layer.h"
#include "Layer/MainLayer.h"
#include "Layer/ToolLayer.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Output/Image2DWriter.h"
#include "Core/PythonHelper.h"

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
    if (m_loaderThread.joinable())
        m_loaderThread.join();

    m_layerManager.DropAllLayers();
    GetRenderer().Terminate();
    FinalizePython();
    ULOGD("Application terminated");
}

void Application::Run()
{
    while (m_window.IsOpen())
    {        
        if (Application::GetKeyboard().IsKeyPressed(Keyboard::Key::ESCAPE))
        {
            m_window.Close();
        }
        
        m_mouse.UpdateCursorMode(m_window);

        m_window.PullEvents();
        HandleEvents();
        UpdateAndRenderLayers();
    }
}

void Application::UpdateAndRenderLayers()
{
    auto& layers = m_layerManager.GetLayers();

    for (auto layer : layers)
        layer->Update();

    if (!m_window.IsMinimized())
    {
        for (auto layer : layers)
            layer->RenderLayerComponents();
        
        m_window.Clear();

        for (auto layer : layers)
            layer->Render();

        m_window.SwapBuffers();
    }
}

void Application::InitApplciation()
{
    InitializePython();

    auto& renderer = GetRenderer();
    renderer.SetApi(Ull::Renderer::API::OPEN_GL);

    //Window
    if (glfwInit() == -1)
        throw InitializationException("Can't initialize GLFW");

    m_window.Create("Ullmanite 0.7v", glm::uvec2(1280, 720));
    m_window.SetEventQueueDataPointer(&m_eventQueue);
    m_window.SetRefreshFunction([this]() { WindowRefreshFunction(); });

    //Renderer
    glfwMakeContextCurrent(m_window.GetWindowContext());

    renderer.Init();
    renderer.SetViewPort(glm::uvec2(0, 0), m_window.GetSize());

    //ImGui
    const char* glsl_version = "#version 140";

    IMGUI_CHECKVERSION();
    const auto context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    const auto state = ImGui_ImplGlfw_InitForOpenGL(m_window.GetWindowContext(), true);
    const auto state2 = ImGui_ImplOpenGL3_Init(glsl_version);

    //Load Shaders
    auto& shaderManager = renderer.GetShaderManager();

    shaderManager.LoadShader(ShaderTag::UI_BASIC_COLOR, "UiBasicVS", "UiBasicColorPS");
    shaderManager.LoadShader(ShaderTag::FRAME_DISPLAY_SHADER, "DisplayFrameVS", "DisplayFramePS");
    shaderManager.LoadShader(ShaderTag::MARKER, "MarkerVS", "MarkerPS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_MESH_GENERATOR, "CubeMarchCS");
    shaderManager.LoadShader(ShaderTag::TRANSFER_FUNCTION_GENERATOR, "TransferFunctionGeneratorCS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_VERTEX_COUNTER, "CubeMarchVertexCounterCS");
    shaderManager.LoadShader(ShaderTag::CUBE_MARCH_VERTEX_RENDERER, "CubeMarchVS", "CubeMarchPS", "CubeMarchGS");
    shaderManager.LoadShader(ShaderTag::UI_GRADIENT_SHADER, "UiGradientVS", "UiGradientPS");
    shaderManager.LoadShader(ShaderTag::UI_GRADIENT_SHADER_HSV, "UiGradientVS", "UiGradientHsvPS");
    shaderManager.LoadShader(ShaderTag::SIGNED_DISTANCE_FIELD_FOR_2D_TEXTURES, "SignedDistanceField2DCS");
    shaderManager.LoadShader(ShaderTag::INVERSE_2D_BIT_MAP, "Inverse2DBitMapCS");
    shaderManager.LoadShader(ShaderTag::MEREGE_INNSER_OUTER_SDF, "MergeInnerOuterSdfCS");
    shaderManager.LoadShader(ShaderTag::SDF_TEXT, "SdfTextVS", "SdfTextPS");

    //Resources
    auto& fontManager = m_resourceManager.GetFontMnager();
    fontManager.InitLoader();
    fontManager.LoadFont("segoeui.ttf", FontTag::UI_FONT, 128, 33, 126);
    fontManager.LoadFont("UllIcon.ttf", FontTag::UI_ICON, 256, 61440, 61449);
    fontManager.ReleaseLoader();

    TriangulationTable::GetInstance().CreateTriangulationTable();
    TriangulationTable::GetInstance().CreateVectexCountTable();

    //Layers
    auto& window = Application::GetWindow();
    auto mainLayer = std::make_shared<MainLayer>(window.GetSize(), NotOwner<LayerManager>(&m_layerManager));
    m_layerManager.PushLayer(mainLayer);

    auto toolLayer = std::make_shared<ToolLayer>(window.GetSize(), NotOwner<LayerManager>(&m_layerManager));
    m_layerManager.PushLayer(toolLayer);

    //First resizeEvent to inform components of the initial window size
    const auto windowSize = m_window.GetSize();
    m_eventQueue.PushEvent(std::make_shared<WindowResizeEvent>(EventType::WindowResize, glm::uvec2(windowSize.x, windowSize.y)));
}

void Application::HandleEvents()
{
    m_eventQueue.MakeEventUnique(EventType::WindowResize);

    std::vector<Keyboard::KeyState> newKeyStates{};
    std::vector<Mouse::ButtonState> newButtonStates{};
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
        {
            Keyboard::KeyState keyState;
            keyState.key = static_cast<KeyDownEvent*>(currentEvent.get())->GetVal();
            keyState.state = true;
            newKeyStates.push_back(keyState);
            break;
        }
        case EventType::KeyUp:
        {
            Keyboard::KeyState keyState;
            keyState.key = static_cast<KeyUpEvent*>(currentEvent.get())->GetVal();
            keyState.state = false;
            newKeyStates.push_back(keyState);
            break;
        }
        case EventType::MouseDown:
        {
            Mouse::ButtonState buttonState;
            buttonState.button = static_cast<MouseDownEvent*>(currentEvent.get())->GetVal();
            buttonState.state = true;
            newButtonStates.push_back(buttonState);
            break;
        }
        case EventType::MouseUp:
        {
            Mouse::ButtonState buttonState;
            buttonState.button = static_cast<MouseDownEvent*>(currentEvent.get())->GetVal();
            buttonState.state = false;
            newButtonStates.push_back(buttonState);
            break;
        }
        case EventType::MouseMove:
            GetMouse().UpdatePosition(static_cast<MouseMoveEvent*>(currentEvent.get())->GetVal());
            break;

        case EventType::MouseScroll:
            scroll = static_cast<MouseScrollEvent*>(currentEvent.get())->GetVal();
            break;

        default:
            break;
        case EventType::DataFolderSelected:
            const auto event = static_cast<DataFolderSelectedEvent*>(currentEvent.get());
            const auto folderPath = event->GetVal();

            if (m_loaderThread.joinable())
                m_loaderThread.join();

            m_loaderThread = std::thread([this](const std::wstring& path) {
                const auto success = m_resourceManager.GetVolumeManager().LoadVolumeFromFolder(path);
                if (success)
                    m_eventQueue.PushEvent(std::make_shared<VolumeLoadedEvent>(EventType::VolumeLoaded));
            }, folderPath);
                
            break;
        }

         m_window.HandleEvent(currentEvent.get());
         m_layerManager.HandleEvent(currentEvent.get());
    }

    for (const auto keyState : newKeyStates)
        GetKeyboard().UpdateKeyMap(keyState);

    for (const auto buttonState : newButtonStates)
        GetMouse().UpdateButtonMap(buttonState);

    GetMouse().UpdateScroll(scroll);
}

void Application::WindowResizeHandler(const glm::uvec2& size)
{
    GetRenderer().SetViewPort(glm::uvec2(0, 0), size);
}

void Application::WindowRefreshFunction()
{
    HandleEvents();
    UpdateAndRenderLayers();
}
