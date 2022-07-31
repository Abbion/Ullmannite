#pragma once
#include <optional>
#include <glm/glm.hpp>

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
        MouseUp
    };

    //==================================================================
    class Event
    {
    public:
        Event(EventType eventType) : m_type(eventType) {}
        virtual ~Event() {}

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
};