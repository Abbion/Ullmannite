#pragma once
#include <vector>
#include <memory>
#include "Event/EventHandler.h"

namespace Ull
{
    class Scene;

    class Node : public EventHandler
    {
    public:
        Node(const std::string& name, const Scene* scene);
        virtual ~Node();

        Node* GetParent() const { return m_parent; }
        std::vector<Node*> GetChildren() { return m_children; }
        const Scene* GetScene() const { return m_scene; }
        const std::string GetName() const { return m_name; }

        void SetName(const std::string& name) { m_name = name; }
        
        void SetParent(Node* node);
        void AddNode(Node* node);
        bool RemoveNode(Node* node);

        virtual void HandleEvent(Event* event) override {};
        virtual void Update() {};

        bool operator==(Node* node);
        bool operator!=(Node* node);

    protected:
        unsigned long long m_id; //TODO: Think if this is needed if we have name

        Node* m_parent;
        std::vector<Node*> m_children;
        const Scene* m_scene;
        std::string m_name;
    };
}