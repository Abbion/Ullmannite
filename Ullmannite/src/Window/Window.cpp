#include "Ullpch.h"
#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Logger/Logger.h"
#include "Event/Event.h"

#define MIN_WINDOW_WIDTH 1024
#define MIN_WINDOW_HEIGHT 576

using namespace Ull;

Window::Window(std::string title, glm::uvec2 size) :
    m_size(std::move(size)),
    m_title(std::move(title))
{
    if(m_size.x < MIN_WINDOW_WIDTH)
        m_size.x = MIN_WINDOW_WIDTH;

    if(m_size.y < MIN_WINDOW_HEIGHT)
        m_size.y = MIN_WINDOW_HEIGHT;

    m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);

    if (!m_window)
        throw std::exception("Can't initialize Window");

    glfwMakeContextCurrent(m_window);
    glfwGetWindowPos(m_window, &m_position.x, &m_position.y);

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

void Window::SetSize(const glm::uvec2& size)
{
    m_size = size;

    if(m_size.x < MIN_WINDOW_WIDTH)
        m_size.x = MIN_WINDOW_WIDTH;

    if(m_size.y < MIN_WINDOW_HEIGHT)
        m_size.y = MIN_WINDOW_HEIGHT;

    glfwSetWindowSize(m_window, m_size.x, m_size.y);
}

void Window::SetPosition(const glm::ivec2& position)
{
    m_position = position;
    glfwSetWindowPos(m_window, m_position.x, m_position.y);
}

void Window::Minimize()
{
    m_isMinimized = true;
    glfwIconifyWindow(m_window);
}

void Window::Maximize()
{
    m_isMinimized = false;
    glfwMaximizeWindow(m_window);
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
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int positionX, int positionY)
        {
            auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
            eventQueue->PushEvent(std::make_shared<WindowMoveEvent>(glm::uvec2(positionX, positionY)));
        });

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
            eventQueue->PushEvent(std::make_shared<WindowResizeEvent>(glm::uvec2(width, height)));
        });
    
    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        
        if(focused)
            eventQueue->PushEvent(std::make_shared<WindowGainedFocusEvent>());
        else
            eventQueue->PushEvent(std::make_shared<WindowLostFocusEvent>());
    });

    glfwSetWindowIconifyCallback(m_window, [](GLFWwindow* window, int iconified){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(iconified)
            eventQueue->PushEvent(std::make_shared<WindowMinimizedEvent>());
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>());
    });

    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* window, int maximized){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(maximized)
            eventQueue->PushEvent(std::make_shared<WindowMaximizedEvent>());
        else
            eventQueue->PushEvent(std::make_shared<WindowRestoredEvent>());
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<WindowClosedEvent>());
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if(action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<KeyDownEvent>(static_cast<Keyboard::Key>(key)));
        else if(action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<KeyUpEvent>(static_cast<Keyboard::Key>(key)));
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS)
            eventQueue->PushEvent(std::make_shared<MouseDownEvent>(static_cast<Mouse::Button>(button)));
        else if (action == GLFW_RELEASE)
            eventQueue->PushEvent(std::make_shared<MouseUpEvent>(static_cast<Mouse::Button>(button)));
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseMoveEvent>(glm::ivec2(xpos, ypos)));
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        auto eventQueue = reinterpret_cast<EventQueue*>(glfwGetWindowUserPointer(window));
        eventQueue->PushEvent(std::make_shared<MouseScrollEvent>(static_cast<int>(yoffset)));
    });
}