#pragma once
#include "Event/Event.h"
#include "Event/EventHandler.h"
#include "UiElement/UiLayout.h"

namespace Ull
{
    class Layer : public EventHandler
    {
    public:
        NON_COPYABLE(Layer)
        
        virtual ~Layer();
        
        inline std::string GetName() const { return m_name; }
        const std::shared_ptr<UiLayout> GetLayout() const { return m_layout; }

        virtual void Update() = 0;
        virtual void Render() = 0;

    protected:
        std::shared_ptr<UiLayout> m_layout;
        unsigned int m_focusedElement { 0 };
    
    protected:
        Layer(const std::string& name, const glm::uvec2& size);     //Constructor

    private:
        std::string m_name;
    };
};