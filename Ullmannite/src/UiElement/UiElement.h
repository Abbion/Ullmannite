#pragma once
#include "glm/glm.hpp"
#include "Event/EventHandler.h"

namespace Ull
{
    class UiElement : public EventHandler
    {
    public:
        UiElement(std::string name, glm::uvec2 position, glm::uvec2 size) :
            m_name(name), m_position(position), m_size(size) {}
        virtual ~UiElement();

        virtual void CreateResources() = 0;

        virtual void Update() = 0;
        virtual void Render() = 0;

        void HandleEvent(Event* event) override;

        void AddUiElement(UiElement* newElement);
        UiElement* GetParent() { return m_parent; }
        std::vector<UiElement*>& GetChildren() { return m_children; }

        glm::uvec2 GetPosition() const      { return m_position; }
        glm::uvec2 GetSize() const          { return m_size; }
        float GetScale() const              { return m_scale; }
        std::string GetName() const         { return m_name; }

        void SetPositiion(const glm::uvec2& position)   { m_position = position; }
        void SetSize(const glm::uvec2& size)            { m_size = size; }
        void SetScale(const float& scale)               { m_scale = scale; }

    protected:
        glm::uvec2 m_position;
        glm::uvec2 m_size;
        float m_scale{ 1.0f };

        std::string m_name;

        UiElement* m_parent{ nullptr };
        std::vector<UiElement*> m_children;
    };
}