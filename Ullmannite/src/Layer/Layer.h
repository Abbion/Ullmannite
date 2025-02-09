#pragma once
#include "Event/Event.h"
#include "Event/EventHandler.h"
#include "UiElement/UiLayout.h"
#include "UiElement/UiRenderArea.h"

namespace Ull
{
    // Add something to this class because it looks like UiLayout can be used instead of it
    // Add a focused element for example
    class Layer : public EventHandler
    {
    public:
        NON_COPYABLE(Layer);

        void Update();
        virtual void Render();

        void SetVisibility(const bool state) { m_layout->SetVisibility(state); }
        bool IsVisible() const { return m_layout->IsVisible(); }

        const std::shared_ptr<UiLayout> GetLayout() const { return m_layout; }

    protected:
        std::shared_ptr<UiLayout> m_layout;
    
    protected:
        Layer(const std::string& name, const glm::uvec2 size);
    };
};