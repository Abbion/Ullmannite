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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
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
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(eventQueue));
}

void UllWindow::HandleEvent(Event* event)
{
    if(event->IsHandeled())
        return;
    
    if(event->GetType() == EventType::KeyDown)
    {
        if(static_cast<KeyDownEvent*>(event)->GetVal() == Keyboard::Key::F)
        {
            if(Keyboard::GetInstance().IsKeyPressed(Keyboard::Key::L_CONTROL))
                SwitchHiddenCursor();
        }
    }
}

glm::ivec2 UllWindow::GetPosition() const
{
    glm::ivec2 position;
    glfwGetWindowPos(m_window, &position.x, &position.y);
    
    return position;
}

glm::ivec2 UllWindow::GetSize() const
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

void UllWindow::CheckCursorInteractions()
{
    MovedByCursor();
    CheckResizeBorder();
    ResizeByCursor();
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
    if (m_isResized)
    {
        Renderer::GetInstance().FlushContext();
        glfwSwapInterval(2);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);
        m_intervalRestored = false;

        //Synchronization for resizing
        auto now = std::chrono::steady_clock::now();
        auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastRefresh);
        auto waitForMs = 20 - durationMs.count();

        if (waitForMs > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(waitForMs));
        m_lastRefresh = std::chrono::steady_clock::now();
    }
    else
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);

        if (m_intervalRestored)
        {
            glfwSwapInterval(0);
            m_intervalRestored = true;
        }
    }
}

void UllWindow::PullEvents()
{
    glfwPollEvents();
}

void UllWindow::CheckResizeBorder()
{
    if(m_isDragged)
        return;
    
    if (!m_isResized)
    {
        glm::dvec2 cursorPos;
        glfwGetCursorPos(m_window, &cursorPos.x, &cursorPos.y);

        auto bottmRightCorner = GetPosition() + GetSize();
        m_windowPositionWithMinSize.x = bottmRightCorner.x - MIN_WINDOW_WIDTH;
        m_windowPositionWithMinSize.y = bottmRightCorner.y - MIN_WINDOW_HEIGHT;

        auto windowSize = GetSize();
        auto cornerBias = m_resizeBorderSize * 2.0f;

        if ((cursorPos.x < cornerBias && cursorPos.y < cornerBias))
        {
            m_resizeBorder = ResizeFrame::TOP_LEFT;
            ChangeCursorShape(GLFW_RESIZE_NWSE_CURSOR);
        }
        else if ((cursorPos.x > windowSize.x - cornerBias && cursorPos.y > windowSize.y - cornerBias))
        {
            m_resizeBorder = ResizeFrame::BOTTOM_RIGHT;
            ChangeCursorShape(GLFW_RESIZE_NWSE_CURSOR);
        }
        else if ((cursorPos.x < cornerBias && cursorPos.y > windowSize.y - cornerBias))
        {
            m_resizeBorder = ResizeFrame::BOTTOM_LEFT;
            ChangeCursorShape(GLFW_RESIZE_NESW_CURSOR);
        }
        else if ((cursorPos.x > windowSize.x - cornerBias && cursorPos.y < cornerBias))
        {
            m_resizeBorder = ResizeFrame::TOP_RIGHT;
            ChangeCursorShape(GLFW_RESIZE_NESW_CURSOR);
        }
        else if (cursorPos.x < m_resizeBorderSize)
        {
            m_resizeBorder = ResizeFrame::LEFT;
            ChangeCursorShape(GLFW_RESIZE_EW_CURSOR);
        }
        else if (cursorPos.x > windowSize.x - m_resizeBorderSize)
        {
            m_resizeBorder = ResizeFrame::RIGHT;
            ChangeCursorShape(GLFW_RESIZE_EW_CURSOR);
        }
        else if (cursorPos.y < m_resizeBorderSize)
        {
            m_resizeBorder = ResizeFrame::TOP;
            ChangeCursorShape(GLFW_RESIZE_NS_CURSOR);
        }
        else if (cursorPos.y > windowSize.y - m_resizeBorderSize)
        {
            m_resizeBorder = ResizeFrame::BOTTOM;
            ChangeCursorShape(GLFW_RESIZE_NS_CURSOR);
        }
        else
        {
            m_resizeBorder = ResizeFrame::NONE;
            ChangeCursorShape(GLFW_ARROW_CURSOR);
        }
    }
}

void UllWindow::ResizeByCursor()
{
    if(m_isDragged || m_isMaximized)
        return;

    if (m_resizeBorder != ResizeFrame::NONE && Mouse::GetInstance().IsButtonPressed(Mouse::Button::LEFT))
    {
        if(!m_isResized) //Started resizing
        {
            m_startGrabPosition = GetPosition();
            m_isResized = true;
        }

        glm::ivec2 cursorPosition = GetCursorScreenPosition();

        if (m_resizeBorder == ResizeFrame::TOP_LEFT)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(cursorPosition.x, cursorPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;

            SetSize(windowSize);

            if (newWindowPosition.x > m_windowPositionWithMinSize.x)
                newWindowPosition.x = m_windowPositionWithMinSize.x;

            if (newWindowPosition.y > m_windowPositionWithMinSize.y)
                newWindowPosition.y = m_windowPositionWithMinSize.y;

            SetPosition(newWindowPosition);
        }
        else if (m_resizeBorder == ResizeFrame::TOP_RIGHT)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(windowPosition.x, cursorPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;
            windowSize.x = cursorPosition.x - m_startGrabPosition.x;

            SetSize(windowSize);

            if (newWindowPosition.y > m_windowPositionWithMinSize.y)
                newWindowPosition.y = m_windowPositionWithMinSize.y;

            SetPosition(newWindowPosition);
        }
        else if (m_resizeBorder == ResizeFrame::BOTTOM_LEFT)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(cursorPosition.x, windowPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;
            windowSize.y = cursorPosition.y - m_startGrabPosition.y;
            
            SetSize(windowSize);

            if (newWindowPosition.x > m_windowPositionWithMinSize.x)
                newWindowPosition.x = m_windowPositionWithMinSize.x;

            SetPosition(newWindowPosition);
        }
        else if (m_resizeBorder == ResizeFrame::BOTTOM_RIGHT)
        {
            glm::ivec2 windowSize(cursorPosition.x - m_startGrabPosition.x, cursorPosition.y - m_startGrabPosition.y);
            SetSize(windowSize);
        }
        else if (m_resizeBorder == ResizeFrame::TOP)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(windowPosition.x, cursorPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;

            SetSize(windowSize);

            if (newWindowPosition.y > m_windowPositionWithMinSize.y)
                newWindowPosition.y = m_windowPositionWithMinSize.y;

            SetPosition(newWindowPosition);
        }
        else if (m_resizeBorder == ResizeFrame::BOTTOM)
        {
            glm::ivec2 windowSize(cursorPosition.x - m_startGrabPosition.x, cursorPosition.y - m_startGrabPosition.y);

            auto windowPosition = GetSize();
            SetSize(glm::ivec2(windowPosition.x, windowSize.y));
        }
        
        else if (m_resizeBorder == ResizeFrame::LEFT)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(cursorPosition.x, windowPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;

            SetSize(windowSize);

            if (newWindowPosition.x > m_windowPositionWithMinSize.x)
                newWindowPosition.x = m_windowPositionWithMinSize.x;

            SetPosition(newWindowPosition);
        }
        else if (m_resizeBorder == ResizeFrame::RIGHT)
        {
            glm::ivec2 windowSize(cursorPosition.x - m_startGrabPosition.x, cursorPosition.y - m_startGrabPosition.y);

            auto windowPosition = GetSize();
            SetSize(glm::ivec2(windowSize.x, windowPosition.y));
        }
    }
    else
    {
        m_isResized = false;
    }
}

void UllWindow::MovedByCursor()
{
    if(m_isResized)
        return;

    if ((m_dragEnabled || m_isDragged) && m_resizeBorder == ResizeFrame::NONE && Mouse::GetInstance().IsButtonPressed(Mouse::Button::LEFT))
    {
        if (!m_isDragged)   //Started draggins
        {
            glm::dvec2 startGrapPosGetter;
            glfwGetCursorPos(m_window, &startGrapPosGetter.x, &startGrapPosGetter.y);
            m_startGrabPosition = glm::ivec2(startGrapPosGetter);

            if (m_isMaximized)
            {
                //Reposition the window after it was Restored to be grabbet at the same poit as it was when it was maximized
                auto originalSize = GetSize();
                Restore();
                auto afterRestoreSize = GetSize();
                auto sizeScale = (float)afterRestoreSize.x / (float)originalSize.x;
                m_startGrabPosition.x = int((float)m_startGrabPosition.x * sizeScale);
            }

            m_isDragged = true;
        }

        glm::ivec2 newWindowPosition = GetCursorScreenPosition();
        newWindowPosition.x -= m_startGrabPosition.x;
        newWindowPosition.y -= m_startGrabPosition.y;
        SetPosition(newWindowPosition);
    }
    else
    {
        m_isDragged = false;
    }
}

void UllWindow::ChangeCursorShape(int shape)
{
    if (m_currentCursorShape != shape)
    {
        glfwDestroyCursor(m_cursor);
        m_currentCursorShape = shape;
        m_cursor = glfwCreateStandardCursor(m_currentCursorShape);

        glfwSetCursor(m_window, m_cursor);
    }
}

glm::ivec2 UllWindow::GetCursorScreenPosition()
{
#ifdef  PLATFORM_WINDOWS
    POINT cursorPosOnScreen;
    GetCursorPos(&cursorPosOnScreen);
    return glm::ivec2(cursorPosOnScreen.x, cursorPosOnScreen.y);
#endif 

#ifdef PLATFORM_LINUX
    auto display = glfwGetX11Display();
    auto window = glfwGetX11Window(m_window);

    
    Window windowReturned;
    glm::ivec2 rootPos;
    glm::ivec2 inWindowPos;
    unsigned int maskReturn;

    XQueryPointer(display, window, &windowReturned, &windowReturned, &rootPos.x, &rootPos.y, &inWindowPos.x, &inWindowPos.y, &maskReturn);

    return rootPos;
#endif
}

void UllWindow::InitCallBacks()
{
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int positionX, int positionY){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowMoveEvent>(EventType::WindowMaximized, glm::uvec2(positionX, positionY)));
    });
   
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowResizeEvent>(EventType::WindowResize, glm::uvec2(width, height)));
    });
    
    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        
        if(focused)
            eventQueue->PushEvent(std::make_shared<WindowGainedFocusEvent>(EventType::WindowGainedFocus));
        else
            eventQueue->PushEvent(std::make_shared<WindowLostFocusEvent>(EventType::WindowLostFocus));
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconified){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(iconified)
            eventQueue->PushEvent(std::make_shared<WindowMinimizedEvent>(EventType::WindowMinimized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(maximized)
            eventQueue->PushEvent(std::make_shared<WindowMaximizedEvent>(EventType::WindowMaximized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowClosedEvent>(EventType::WindowClosed));
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<KeyDownEvent>(EventType::KeyDown, static_cast<Keyboard::Key>(key)));
        else if(action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<KeyUpEvent>(EventType::KeyUp, static_cast<Keyboard::Key>(key)));
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<MouseDownEvent>(EventType::MouseDown, static_cast<Mouse::Button>(button)));
        else if (action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<MouseUpEvent>(EventType::MouseUp, static_cast<Mouse::Button>(button)));
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseMoveEvent>(EventType::MouseMove, glm::ivec2(xpos, ypos)));
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseScrollEvent>(EventType::MouseScroll, static_cast<int>(yoffset)));
    });

    glfwSetErrorCallback([](int code, const char* description) {
        ULOGF("GLFW error! " << code << " - " << description);
    });
}