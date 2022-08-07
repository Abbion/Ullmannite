#include "Ullpch.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Logger/Logger.h"
#include "Event/Event.h"

#define MIN_WINDOW_WIDTH 1024
#define MIN_WINDOW_HEIGHT 576

using namespace Ull;

Window::Window(std::string title, glm::uvec2 size)
{
    if(size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if(size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    m_window = glfwCreateWindow(size.x, size.y, m_title.c_str(), nullptr, nullptr);

    if (!m_window)
        throw InitializationException("Can't initialize Window");

    glfwMakeContextCurrent(m_window);

    InitCallBacks();
}

Window::~Window()
{    
    glfwDestroyWindow(m_window);
    glfwTerminate();

    ULOGD("Window terminated");
}

void Window::SetTitle(const std::string& title)
{
    m_title = title;
    glfwSetWindowTitle(m_window, m_title.c_str());
}

void Window::SetSize(glm::uvec2 size)
{
    if(size.x < MIN_WINDOW_WIDTH)
        size.x = MIN_WINDOW_WIDTH;

    if(size.y < MIN_WINDOW_HEIGHT)
        size.y = MIN_WINDOW_HEIGHT;

    glfwSetWindowSize(m_window, size.x, size.y);
}

void Window::SetPosition(glm::ivec2 position)
{
    glfwSetWindowPos(m_window, position.x, position.y);
}

glm::ivec2 Window::GetPosition() const
{
    glm::ivec2 position;
    glfwGetWindowPos(m_window, &position.x, &position.y);
    
    return position;
}

glm::ivec2 Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(m_window, &size.x, &size.y);

    return size;
}

void Window::Close()
{
    m_isOpen = false;
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::PullEvents(EventQueue* eventQueue)
{
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(eventQueue));
    glfwPollEvents();
}

void Window::InitCallBacks()
{
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int positionX, int positionY){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowMoveEvent>(EventType::WindowMaximized, glm::uvec2(positionX, positionY)));
    });
   
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
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
}