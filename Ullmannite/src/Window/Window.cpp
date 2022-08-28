#include "Ullpch.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Rendering/Api/Renderer.h"

#define MIN_WINDOW_WIDTH 1024
#define MIN_WINDOW_HEIGHT 576

#include <Windows.h>

using namespace Ull;

Window::Window(std::string title, glm::uvec2 size) : m_title(title)
{
    if(size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if(size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    if(Renderer::GetInstance()->GetApi() == Renderer::API::OPEN_GL)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    m_eventContext = glfwCreateWindow(1, 1, "eventContext", nullptr, nullptr);

    if (!m_eventContext)
        throw InitializationException("Can't initialize event context");

    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    m_renderWindow = glfwCreateWindow(size.x, size.y, m_title.c_str(), nullptr, m_eventContext);

    if (!m_renderWindow)
        throw InitializationException("Can't initialize Window");

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor != nullptr)
    {
        glm::ivec2 minitorAreaSize;
        glfwGetMonitorWorkarea(primaryMonitor, NULL, NULL, &minitorAreaSize.x, &minitorAreaSize.y);

        glm::ivec2 initAppPosition(minitorAreaSize.x / 2.f - size.x / 2.f, minitorAreaSize.y / 2.f - size.y / 2.f);
        glfwSetWindowPos(m_renderWindow, initAppPosition.x, initAppPosition.y);
    }
    else
    {
        glfwSetWindowPos(m_renderWindow, 50, 50);
    }

    glfwSetWindowSizeLimits(m_renderWindow, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    InitCallBacks();
}

Window::~Window()
{    
    glfwDestroyWindow(m_renderWindow);
    glfwDestroyWindow(m_eventContext);
    glfwTerminate();

    ULOGD("Window terminated");
}

void Window::SetTitle(const std::string& title)
{
    m_title = title;
    glfwSetWindowTitle(m_renderWindow, m_title.c_str());
}

void Window::SetSize(glm::uvec2 size)
{
    if(size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if(size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    glfwSetWindowSize(m_renderWindow, size.x, size.y);
}

void Window::SetPosition(glm::ivec2 position)
{
    glfwSetWindowPos(m_renderWindow, position.x, position.y);
}

void Window::SetEventQueueDataPointer(EventQueue* eventQueue)
{
    glfwSetWindowUserPointer(m_renderWindow, reinterpret_cast<void*>(eventQueue));
}

glm::ivec2 Window::GetPosition() const
{
    glm::ivec2 position;
    glfwGetWindowPos(m_renderWindow, &position.x, &position.y);
    
    return position;
}

glm::ivec2 Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(m_renderWindow, &size.x, &size.y);

    return size;
}

void Window::Close()
{
    m_isOpen = false;
}

void Window::CheckCursorInteractions()
{
    MovedByCursor();
    CheckResizeBorder();
    ResizeByCursor();
}

void Window::CheckResizeBorder()
{
    if(m_isDragged)
        return;
    
    if (!m_isResized)
    {
        glm::dvec2 cursorPos;
        glfwGetCursorPos(m_renderWindow, &cursorPos.x, &cursorPos.y);

        auto windowSize = GetSize();
        auto cornerBias = m_resizeBorderSize * 2.0f;

        if ((cursorPos.x < cornerBias && cursorPos.y < cornerBias))
        {
            m_resizeBorder = ResizeBorder::TOP_LEFT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
        }
        else if ((cursorPos.x > windowSize.x - cornerBias && cursorPos.y > windowSize.y - cornerBias))
        {
            m_resizeBorder = ResizeBorder::BOTTOM_RIGHT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
        }
        else if ((cursorPos.x < cornerBias && cursorPos.y > windowSize.y - cornerBias))
        {
            m_resizeBorder = ResizeBorder::BOTTOM_LEFT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
        }
        else if ((cursorPos.x > windowSize.x - cornerBias && cursorPos.y < cornerBias))
        {
            m_resizeBorder = ResizeBorder::TOP_RIGHT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
        }
        else if (cursorPos.x < m_resizeBorderSize)
        {
            m_resizeBorder = ResizeBorder::LEFT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
        }
        else if (cursorPos.x > windowSize.x - m_resizeBorderSize)
        {
            m_resizeBorder = ResizeBorder::RIGHT;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
        }
        else if (cursorPos.y < m_resizeBorderSize)
        {
            m_resizeBorder = ResizeBorder::TOP;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
        }
        else if (cursorPos.y > windowSize.y - m_resizeBorderSize)
        {
            m_resizeBorder = ResizeBorder::BOTTOM;
            m_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
        }
        else
        {
            m_resizeBorder = ResizeBorder::NONE;
            m_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        }

        glfwSetCursor(m_renderWindow, m_cursor);
    }
}

void Window::ResizeByCursor()
{
    if(m_isDragged)
        return;

    if (m_resizeBorder != ResizeBorder::NONE && Mouse::GetInstance()->IsButtonPressed(Mouse::Button::LEFT))
    {
        if(!m_isResized) //Started resizing
        {
            m_startGrabPosition = GetPosition();
            m_isResized = true;
        }

        glm::ivec2 cursorPosition = GetCursorScreenPosition();

        if (m_resizeBorder == ResizeBorder::TOP_LEFT || m_resizeBorder == ResizeBorder::TOP_RIGHT || m_resizeBorder == ResizeBorder::TOP)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(windowPosition.x, cursorPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;

            if (windowSize.y > MIN_WINDOW_HEIGHT)
            {
                SetSize(windowSize);
                SetPosition(newWindowPosition);
            }
        }
        if (m_resizeBorder == ResizeBorder::BOTTOM_RIGHT || m_resizeBorder == ResizeBorder::BOTTOM_LEFT || m_resizeBorder == ResizeBorder::BOTTOM)
        {
            glm::ivec2 windowSize(cursorPosition.x - m_startGrabPosition.x, cursorPosition.y - m_startGrabPosition.y);

            auto windowPosition = GetSize();
            SetSize(glm::ivec2(windowPosition.x, windowSize.y));
        }
        if (m_resizeBorder == ResizeBorder::BOTTOM_LEFT || m_resizeBorder == ResizeBorder::TOP_LEFT || m_resizeBorder == ResizeBorder::LEFT)
        {
            auto windowPosition = GetPosition();
            auto newWindowPosition = glm::ivec2(cursorPosition.x, windowPosition.y);

            auto positionDelta = windowPosition - newWindowPosition;
            auto windowSize = GetSize() + positionDelta;

            if (windowSize.x > MIN_WINDOW_WIDTH)
            {
                SetSize(windowSize);
                SetPosition(newWindowPosition);
            }
        }
        if (m_resizeBorder == ResizeBorder::BOTTOM_RIGHT || m_resizeBorder == ResizeBorder::TOP_RIGHT || m_resizeBorder == ResizeBorder::RIGHT)
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

void Window::MovedByCursor()
{
    if(m_isResized)
        return;

    if (m_resizeBorder == ResizeBorder::NONE && Mouse::GetInstance()->IsButtonPressed(Mouse::Button::LEFT))
    {
        if (!m_isDragged)   //Started draggins
        {
            glfwGetCursorPos(m_renderWindow, &m_startGrabPosition.x, &m_startGrabPosition.y);
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

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_renderWindow);
}

void Window::PullEvents()
{
    glfwWaitEvents();
}

glm::ivec2 Window::GetCursorScreenPosition()
{
    //TODO: if windows

    POINT cursorPosOnScreen;
    GetCursorPos(&cursorPosOnScreen);
    return glm::ivec2(cursorPosOnScreen.x, cursorPosOnScreen.y);
}

void Window::InitCallBacks()
{
    glfwSetWindowPosCallback(m_renderWindow, [](GLFWwindow* window, int positionX, int positionY){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowMoveEvent>(EventType::WindowMaximized, glm::uvec2(positionX, positionY)));
    });
   
    glfwSetWindowSizeCallback(m_renderWindow, [](GLFWwindow* window, int width, int height){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowResizeEvent>(EventType::WindowResize, glm::uvec2(width, height)));
    });
    
    glfwSetWindowFocusCallback(m_renderWindow, [](GLFWwindow* window, int focused){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        
        if(focused)
            eventQueue->PushEvent(std::make_shared<WindowGainedFocusEvent>(EventType::WindowGainedFocus));
        else
            eventQueue->PushEvent(std::make_shared<WindowLostFocusEvent>(EventType::WindowLostFocus));
    });

    glfwSetWindowIconifyCallback(m_renderWindow, [](GLFWwindow* window, int iconified){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(iconified)
            eventQueue->PushEvent(std::make_shared<WindowMinimizedEvent>(EventType::WindowMinimized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowMaximizeCallback(m_renderWindow, [](GLFWwindow* window, int maximized){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(maximized)
            eventQueue->PushEvent(std::make_shared<WindowMaximizedEvent>(EventType::WindowMaximized));
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>(EventType::WindowRestored));
    });

    glfwSetWindowCloseCallback(m_renderWindow, [](GLFWwindow* window){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowClosedEvent>(EventType::WindowClosed));
    });

    glfwSetKeyCallback(m_renderWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<KeyDownEvent>(EventType::KeyDown, static_cast<Keyboard::Key>(key)));
        else if(action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<KeyUpEvent>(EventType::KeyUp, static_cast<Keyboard::Key>(key)));
    });

    glfwSetMouseButtonCallback(m_renderWindow, [](GLFWwindow* window, int button, int action, int mods) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<MouseDownEvent>(EventType::MouseDown, static_cast<Mouse::Button>(button)));
        else if (action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<MouseUpEvent>(EventType::MouseUp, static_cast<Mouse::Button>(button)));
    });

    glfwSetCursorPosCallback(m_renderWindow, [](GLFWwindow* window, double xpos, double ypos){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseMoveEvent>(EventType::MouseMove, glm::ivec2(xpos, ypos)));
    });

    glfwSetScrollCallback(m_renderWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseScrollEvent>(EventType::MouseScroll, static_cast<int>(yoffset)));
    });

    glfwSetErrorCallback([](int code, const char* description) {
        ULOGF("GLFW error! " << code << " - " << description);
    });
}