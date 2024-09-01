#include "Ullpch.h"
#include "UllWindow.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Rendering/Api/Renderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <thread>

#define MIN_WINDOW_WIDTH 1024
#define MIN_WINDOW_HEIGHT 576

#ifdef  PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef PLATFORM_LINUX
#include <X11/Xlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#endif

using namespace Ull;

namespace
{
    constexpr double DoubleClickDurationWindow = 0.4;
    constexpr float ScaleUpFactor = 1.25f;
    constexpr float ScaleDownFactor = 1.f / ScaleUpFactor;
    constexpr int MaxScaleCounter = 3;
}

UllWindow::UllWindow()
{
}

void UllWindow::Create(std::string title, glm::uvec2 size)
{
    m_title = title;

    if (size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if (size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    if (Renderer::GetInstance().GetApi() == Renderer::API::OPEN_GL)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_BORDERLESS_AREO, GLFW_TRUE);
    }

    m_window = glfwCreateWindow(size.x, size.y, m_title.c_str(), nullptr, nullptr);

    if (!m_window)
        throw InitializationException("Can't initialize Window");

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor != nullptr)
    {
        glm::ivec2 minitorAreaSize;
        glfwGetMonitorWorkarea(primaryMonitor, NULL, NULL, &minitorAreaSize.x, &minitorAreaSize.y);

        glm::ivec2 initAppPosition(minitorAreaSize.x / 2.f - size.x / 2.f, minitorAreaSize.y / 2.f - size.y / 2.f);
        glfwSetWindowPos(m_window, initAppPosition.x, initAppPosition.y);
    }
    else
    {
        glfwSetWindowPos(m_window, 50, 50);
    }

    glfwSetWindowSizeLimits(m_window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    InitCallBacks();

    m_lastRefresh = std::chrono::steady_clock::now();
}

UllWindow::~UllWindow()
{    
    glfwDestroyWindow(m_window);
    glfwTerminate();

    ULOGD("Window terminated");
}

void UllWindow::SetTitle(const std::string& title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void UllWindow::SetSize(glm::ivec2 size)
{
    if(size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if(size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    glfwSetWindowSize(m_window, size.x, size.y);
}

void UllWindow::SetPosition(glm::ivec2 position)
{
    glfwSetWindowPos(m_window, position.x, position.y);
}

void UllWindow::SetEventQueueDataPointer(EventQueue* eventQueue)
{
    m_windowPointerDataStruct.eventQueue = eventQueue;
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(&m_windowPointerDataStruct));
}

void UllWindow::SetRefreshFunction(std::function<void()> refreshFunction)
{
    m_windowPointerDataStruct.refreshFunction = refreshFunction;
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(&m_windowPointerDataStruct));
}

void UllWindow::SetDragArea(const glm::uvec2 position, const glm::uvec2 size)
{
    glfwSetWindowBorderlessGrabArea(m_window, position.x, position.y, size.x, size.y);
}

void UllWindow::HandleEvent(Event* event)
{
    if(event->IsHandeled())
        return;
    
    switch (event->GetType())
    {
    case EventType::KeyDown:
        if (static_cast<KeyDownEvent*>(event)->GetVal() == Keyboard::Key::F)
        {
            if (Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::L_CONTROL))
                SwitchHiddenCursor();
        }
    break;

    case EventType::UiScaledUp:
        if (m_scaleCounter < MaxScaleCounter)
            m_scaleCounter++;
        else
            event->MarkHandeled(true);
    break;

    case EventType::UiScaledDown:
        if (m_scaleCounter > -MaxScaleCounter)
            m_scaleCounter--;
        else
            event->MarkHandeled(true);
    break;

    default:
        break;
    }
}

const glm::ivec2 UllWindow::GetPosition() const
{
    glm::ivec2 position;
    glfwGetWindowPos(m_window, &position.x, &position.y);
    
    return position;
}

const glm::ivec2 UllWindow::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(m_window, &size.x, &size.y);

    return size;
}

void UllWindow::Close()
{
    m_isOpen = false;

    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void UllWindow::Maximize()
{
    glfwMaximizeWindow(m_window);
    m_isMaximized = true;
    m_isMinimized = false;
}

void UllWindow::Minimize()
{
    glfwIconifyWindow(m_window);
    m_isMaximized = false;
    m_isMinimized = true;
}

void UllWindow::Restore()
{
    glfwRestoreWindow(m_window);
    m_isMaximized = false;
    m_isMinimized = false;
}

void UllWindow::SwitchHiddenCursor()
{
    if (m_cursorLocked)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_cursorLocked = false;
    }
    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_cursorLocked = true;
    }
}

void UllWindow::SwapBuffers()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void UllWindow::PullEvents()
{
    glfwPollEvents();
}

void UllWindow::InitCallBacks()
{
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int positionX, int positionY){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        eventQueue->PushEvent(std::make_shared<WindowMoveEvent>(EventType::WindowMove, glm::uvec2(positionX, positionY)));
    });
   
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        eventQueue->PushEvent(std::make_shared<WindowResizeEvent>(EventType::WindowResize, glm::uvec2(width, height)));
    });
    
    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        
        if(focused)
            eventQueue->PushEvent(std::make_shared<WindowGainedFocusEvent>(EventType::WindowGainedFocus));
        else
            eventQueue->PushEvent(std::make_shared<WindowLostFocusEvent>(EventType::WindowLostFocus));
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconified){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;

        if(iconified)
            eventQueue->PushEvent(std::make_shared<WindowMinimizedEvent>(EventType::WindowMinimized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;

        if(maximized)
            eventQueue->PushEvent(std::make_shared<WindowMaximizedEvent>(EventType::WindowMaximized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        eventQueue->PushEvent(std::make_shared<WindowClosedEvent>(EventType::WindowClosed));
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;

        if (mods & GLFW_MOD_CONTROL && action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL)
            {
                eventQueue->PushEvent(std::make_shared<UiScaledUpEvent>(EventType::UiScaledUp, ScaleUpFactor));
            }
            else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS)
            {
                eventQueue->PushEvent(std::make_shared<UiScaledDownEvent>(EventType::UiScaledDown, ScaleDownFactor));
            }
        }
        else
        {
            if (action == GLFW_PRESS)
                eventQueue->PushEvent(std::make_shared<KeyDownEvent>(EventType::KeyDown, static_cast<Keyboard::Key>(key)));
            else if (action == GLFW_RELEASE)
                eventQueue->PushEvent(std::make_shared<KeyUpEvent>(EventType::KeyUp, static_cast<Keyboard::Key>(key)));
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;

        static auto lastTimePoint = std::chrono::high_resolution_clock::now();

        if (action == GLFW_PRESS)
        {
            eventQueue->PushEvent(std::make_shared<MouseDownEvent>(EventType::MouseDown, static_cast<Mouse::Button>(button)));
        }
        else if (action == GLFW_RELEASE)
        {
            eventQueue->PushEvent(std::make_shared<MouseUpEvent>(EventType::MouseUp, static_cast<Mouse::Button>(button)));

            if (static_cast<Mouse::Button>(button) == Mouse::Button::LEFT)
            {
                auto timePoint = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = timePoint - lastTimePoint;
                lastTimePoint = timePoint;

                if (duration.count() < DoubleClickDurationWindow)
                {
                    eventQueue->PushEvent(std::make_shared<MouseDoubleUp>(EventType::MouseDoubleUp, static_cast<Mouse::Button>(button)));
                }
            }
        }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        eventQueue->PushEvent(std::make_shared<MouseMoveEvent>(EventType::MouseMove, glm::ivec2(xpos, ypos)));
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;
        eventQueue->PushEvent(std::make_shared<MouseScrollEvent>(EventType::MouseScroll, static_cast<int>(yoffset)));
    });

    glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered) {
        auto eventQueue = reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->eventQueue;

        if (entered)
        {
            eventQueue->PushEvent(std::make_shared<MouseEnteredWindowEvent>(EventType::MouseEnteredWindow));
        }
        else
        {
            eventQueue->PushEvent(std::make_shared<MouseExitedWindowEvent>(EventType::MouseExitedWindow));
        }
    });

    glfwSetWindowRefreshCallback(m_window, [](GLFWwindow* window) {
        reinterpret_cast<WindowPointerDataStruct*>(glfwGetWindowUserPointer(window))->refreshFunction();
    });

    glfwSetErrorCallback([](int code, const char* description) {
        ULOGF("GLFW error! " << code << " - " << description);
    });
}