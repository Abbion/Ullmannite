#include "glm/glm.hpp"
#include "Event/EventHandler.h"

namespace Ull
{
    class UiElement : public EventHandler
    {
    public:
        UiElement(std::string name, glm::uvec2 position, glm::uvec2 size) :
            m_name(name), m_position(position), m_size(size) {}
        ~UiElement() {}

        virtual void Update() = 0;
        virtual void Render() = 0;

    private:
        glm::uvec2 m_position;
        glm::uvec2 m_size;
        std::string m_name;

        UiElement* m_parent{ nullptr };
        std::vector<UiElement*> m_children;
    };
}