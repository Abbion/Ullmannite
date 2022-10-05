#pragma once
#include "Nodes/Node.h"
#include <string>
#include "Event/EventHandler.h"

namespace Ull
{
    class Scene : public EventHandler
    {
    public:
        Scene(const std::string& name);
        ~Scene() {}

        void HandleEvent(Event* event) override {}

        const Node* GetRootNode() const { return m_root; }
        inline const std::string GetName() const { return m_name; } 

    private:
        Node* m_root;

        std::string m_name;
    };
} 
