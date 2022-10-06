#pragma once
#include <vector>
#include <memory>

namespace Ull
{
    class Node
    {
    public:
        Node();
        virtual ~Node();

        Node* GetParent() const { return m_parent; }
        std::vector<Node*> GetChildren() { return m_children; }

        void SetParent(Node* node);
        void AddNode(Node* node);
        bool RemoveNode(Node* node);

        bool operator==(Node* node);
        bool operator!=(Node* node);

    protected:
        unsigned long long m_id;

        Node* m_parent;
        std::vector<Node*> m_children;

        Node* m_root;   //TODO init this
    };
}