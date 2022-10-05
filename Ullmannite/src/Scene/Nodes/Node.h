#pragma once
#include <vector>
#include <memory>

namespace Ull
{
    class Scene;

    class Node
    {
    public:
        Node(const std::shared_ptr<Scene>& scene);
        ~Node();

        const Node* GetParent() const { return m_parent; }
        const std::vector<Node*>& GetChildren() const { return m_children; }

        void SetParent(Node* node);
        void AddNode(Node* node);
        bool RemoveNode(Node* node);

        bool operator==(Node* node);
        bool operator!=(Node* node);

    protected:
        unsigned long long m_id;

        Node* m_parent;
        std::vector<Node*> m_children;

        std::shared_ptr<Scene> m_scene;
    };
}