#pragma once
#include "Nodes/Node.h"
#include <string>
#include "Event/EventHandler.h"
#include "Rendering/DrawInterfaces/Renderable.h"

namespace Ull
{
    class Scene : public EventHandler, public Renderable
    {
    public:
        Scene(std::string name);
        ~Scene();

        void HandleEvent(Event* event) override {}
        void Render() override;

        Node* GetRootNode() const { return m_root; }
        inline const std::string GetName() const { return m_name; } 

    private:
        Node* m_root{ nullptr };
        std::string m_name;
    };
} 
