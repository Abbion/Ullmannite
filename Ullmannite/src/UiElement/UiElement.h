#pragma once
#include "Rendering/Objects/Base/Object2D.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include "Event/EventHandler.h"
#include <DataStructures/Tree/TreeNode.h>

namespace Ull
{
    enum class UiElementType
    {
        Control,
        Area,
        Layout,
        GradientEditor,
        GradientMarker
    };

    class UiElement : public TreeNode<UiElement>, public Object2D, public EventHandler, public Drawable
    {
    public:
        std::optional<NotOwner<UiElement>> FindUiElementAboveByType(const UiElementType type);

        virtual glm::mat4 GetTransform() const override final;

        virtual void HandleEvent(Event* event);

        virtual void Update();
        virtual void Render();

    protected:
        UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size, const UiElementType type);

        virtual void CreateResources() = 0;

    private:
        const UiElementType m_uiElementType;
    };
}