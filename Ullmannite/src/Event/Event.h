#pragma once
#include <optional>
#include <glm/glm.hpp>
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Logger/Logger.h"

namespace Ull
{
    //==================================================================
    enum class EventType
    {
        WindowMove,
        WindowResize,
        WindowLostFocus,
        WindowGainedFocus,
        WindowMaximized,
        WindowMinimized,
        WindowRestored,
        WindowClosed,
        
        KeyDown,
        KeyUp,

        MouseDown,
        MouseUp,
        MouseScroll,
        MouseMove
    };

    //==================================================================
    class Event
    {
    public:
        Event(EventType eventType) : m_type(eventType) {}
        virtual ~Event() { /*ULOGD("event " << (int)m_type << " terminated");*/ }

        EventType GetType() const { return m_type; }

    private:
        EventType m_type;
    };

    //==================================================================
    class WindowMoveEvent : public Event
    {
        public:
            WindowMoveEvent(glm::ivec2 position) : m_position(position), Event(EventType::WindowMove) {}

            glm::ivec2 GetPosition() const { return m_position; }

        private:
            glm::ivec2 m_position;
    };

    //==================================================================
    class WindowResizeEvent : public Event
    {
        public:
            WindowResizeEvent(glm::uvec2 size) : m_size(size), Event(EventType::WindowResize) {}

            glm::uvec2 GetSize() const { return m_size; }

        private:
            glm::uvec2 m_size;
    };

    //==================================================================
    class WindowLostFocusEvent : public Event
    {
        public: 
            WindowLostFocusEvent() : Event(EventType::WindowLostFocus) {}
    };

    //==================================================================
    class WindowGainedFocusEvent : public Event
    {
        public: 
            WindowGainedFocusEvent() : Event(EventType::WindowGainedFocus) {}
    };

     //==================================================================
    class WindowMaximizedEvent : public Event
    {
        public: 
            WindowMaximizedEvent() : Event(EventType::WindowMaximized) {}
    };

    //==================================================================
    class WindowMinimizedEvent : public Event
    {
        public: 
            WindowMinimizedEvent() : Event(EventType::WindowMinimized) {}
    };

    //==================================================================
    class WindowRestoredEvent : public Event
    {
        public: 
            WindowRestoredEvent() : Event(EventType::WindowRestored) {}
    };

    //==================================================================
    class WindowClosedEvent : public Event
    {
        public: 
            WindowClosedEvent() : Event(EventType::WindowClosed) {}
    };  

    //==================================================================
    class KeyDownEvent : public Event
    {
        public: 
            KeyDownEvent(Keyboard::Key key) : m_key(key), Event(EventType::KeyDown) {}

            Keyboard::Key GetKey() const { return m_key; }

        private:
            Keyboard::Key m_key;
    };

    //==================================================================
    class KeyUpEvent : public Event
    {
        public: 
            KeyUpEvent(Keyboard::Key key) : m_key(key), Event(EventType::KeyUp) {}

            Keyboard::Key GetKey() const { return m_key; }
            
        private:
            Keyboard::Key m_key;
    };

    //==================================================================
    class MouseDownEvent : public Event
    {
        public: 
            MouseDownEvent(Mouse::Button button) : m_button(button), Event(EventType::MouseDown) {}

            Mouse::Button GetButton() const { return m_button; }
            
        private:
            Mouse::Button m_button;
    };

    //==================================================================
    class MouseUpEvent : public Event
    {
        public: 
            MouseUpEvent(Mouse::Button button) : m_button(button), Event(EventType::MouseUp) {}

            Mouse::Button GetButton() const { return m_button; }
            
        private:
            Mouse::Button m_button;
    };

    //==================================================================
    class MouseScrollEvent : public Event
    {
        public: 
            MouseScrollEvent(int scroll) : m_scroll(scroll), Event(EventType::MouseScroll) {}

            int GetScroll() const { return m_scroll; }
            
        private:
            int m_scroll;
    };

    //==================================================================
    class MouseMoveEvent : public Event
    {
        public: 
            MouseMoveEvent(glm::ivec2 position) : m_position(position), Event(EventType::MouseMove) {}

            glm::ivec2 GetPosition() const { return m_position; }
            
        private:
            glm::ivec2 m_position;
    };
};