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
        RenderArea,
        Layout,
        GradientEditor,
        GradientMarker,
        TitleBar,
        ScenView
    };

    class UiElement : public TreeNode<UiElement>, std::enable_shared_from_this<UiElement>, public Object2D, public EventHandler, public Drawable
    {
    public:
        std::optional<NotOwner<UiElement>> FindUiElementAboveByType(const UiElementType type);
        const UiElementType GetType() const { return m_uiElementType; }

        virtual glm::vec2 GetGlobalPosition() const override final;
        virtual glm::vec2 GetRenderAreaPosition() const override final;
        virtual glm::mat4 GetTransform() const override final;

        virtual void HandleEvent(Event* event);

        virtual void Update();
        virtual void Render();

    protected:
        UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size, const UiElementType type);

    private:
        const UiElementType m_uiElementType;
    };
}