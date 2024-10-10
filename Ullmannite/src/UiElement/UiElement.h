#pragma once
#include "Rendering/Objects/Base/Object2D.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include "Event/EventHandler.h"

namespace Ull
{
    class UiElement;
    typedef std::shared_ptr<UiElement> UiElementRef;

    class UiElement : public Object2D, public EventHandler, public Drawable
    {
    public:
        using Object2D::Object2D;

        virtual void CreateResources() = 0;

        void AddChildNode(UiElementRef childNode);
        std::vector<UiElementRef>& GetChildren();

        virtual void HandleEvent(Event* event);

        virtual void Update();
        virtual void Render();
    };
}