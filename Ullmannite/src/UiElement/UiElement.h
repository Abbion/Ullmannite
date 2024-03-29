#pragma once
#include "glm/glm.hpp"
#include "Event/EventHandler.h"
#include "Rendering/DrawInterfaces/Drawable.h"
#include "Utilities/PointerHelper.h"

namespace Ull
{
    class UiElement : public EventHandler, public Drawable
    {
    public:
        UiElement(std::string name, glm::uvec2 position, glm::uvec2 size) :
            m_name(name), m_position(position), m_size(size) {}
        virtual ~UiElement();

        virtual void CreateResources() = 0;

        void AddUiElement(UiElement* newElement);
        NotOwner<UiElement> GetParent() const                   { return m_parent; }
        const std::vector<UiElement*>& GetChildren() const      { return m_children; }

        void SetPositiion(const glm::uvec2& position)   { m_position = position; }
        void SetSize(const glm::uvec2& size)            { m_size = size; }
        void SetScale(const float& scale)               { m_scale = scale; }

        glm::uvec2 GetPosition() const              { return m_position; }
        glm::uvec2 GetSize() const                  { return m_size; }
        float GetScale() const                      { return m_scale; }
        inline const std::string GetName() const    { return m_name; }

        void HandleEvent(Event* event) override;
        virtual void Update() = 0;
        void Render() override {}

    protected:
        glm::uvec2 m_position;
        glm::uvec2 m_size;
        float m_scale{ 1.0f };

        std::string m_name;

        NotOwner<UiElement> m_parent{ nullptr };
        std::vector<UiElement*> m_children;

        using Drawable::Drawable;
    };
}