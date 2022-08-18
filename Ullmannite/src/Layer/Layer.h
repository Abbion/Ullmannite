#pragma once
#include "Ullpch.h"
#include "Logger/Logger.h"
#include "Event/Event.h"
#include "Event/EventHandler.h"
#include "UiElement/UiArea.h"

namespace Ull
{
    class Layer : public EventHandler
    {
    public:
        NON_COPYABLE(Layer)

        virtual ~Layer();
        
        virtual void Update() = 0;
        virtual void Render() = 0;

        std::string GetName() const { return m_name; }

    private:
        std::string m_name;

    protected:
        Layer(const std::string& name, const glm::uvec2& size);

        UiArea* m_layout;
        unsigned int m_focusedElement { 0 };
    };
};

//IDEAS:
//Replace name with an enum
//Less writing, but if we have to many layers it becomes harder to read