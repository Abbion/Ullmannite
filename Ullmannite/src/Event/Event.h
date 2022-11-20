#pragma once
#include "Logger/Logger.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "DataStructures/CuttingSettings.h"

#include <glm/glm.hpp>
#include <memory>

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
        MouseMove,

        FileLoaded,
        CuttingSettingsChanged,
        TransferFunctionChanged,
        ExaminationThresholdChanged
    };

    //==================================================================
    class Event : public std::enable_shared_from_this<Event>
    {
    public:
        Event(EventType eventType) : m_type(eventType) {}
        virtual ~Event() { /*ULOGD("event " << (int)m_type << " terminated");*/ }

        void MarkHandeled(bool handeled) { m_handeled = handeled; } 

        EventType GetType() const { return m_type; }
        bool IsHandeled() const { return m_handeled; }

    private:
        EventType m_type;
        bool m_handeled{ false };
    };

    //==================================================================
    template<typename T>
    class ValueEvent : public Event
    {
    public:
        ValueEvent(EventType eventType, T val) : Event(eventType), m_val(val) {}

        T GetVal() const { return m_val; }

    private:
        T m_val;
    };

    //==================================================================
    typedef ValueEvent<glm::ivec2> WindowMoveEvent;
    typedef ValueEvent<glm::uvec2> WindowResizeEvent;
    typedef Event WindowLostFocusEvent;
    typedef Event WindowGainedFocusEvent;
    typedef Event WindowMaximizedEvent;
    typedef Event WindowMinimizedEvent;
    typedef Event WindowRestoredEvent;
    typedef Event WindowClosedEvent;
    typedef Event WindowMinimized;
    typedef Event WindowRestored;

    typedef ValueEvent<Keyboard::Key> KeyDownEvent;
    typedef ValueEvent<Keyboard::Key> KeyUpEvent;

    typedef ValueEvent<Mouse::Button> MouseDownEvent;
    typedef ValueEvent<Mouse::Button> MouseUpEvent;
    typedef ValueEvent<int> MouseScrollEvent;
    typedef ValueEvent<glm::ivec2> MouseMoveEvent;

    typedef ValueEvent<std::string> DataLoadEvent;
    typedef ValueEvent<glm::uvec2> ExaminationThresholdChangedEvent;
    typedef ValueEvent<CuttingSettings> CuttingSettingsChangedEvent;
};