#pragma once
#include "Rendering/Objects/Base/Object2D.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include "Event/EventHandler.h"
#include <DataStructures/Tree/TreeNode.h>

namespace Ull
{
    class UiElement : public TreeNode<UiElement>, public Object2D, public EventHandler, public Drawable
    {
    public:
        UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size);

        virtual void CreateResources() = 0;

        virtual void HandleEvent(Event* event);

        virtual void Update();
        virtual void Render();
    };
}