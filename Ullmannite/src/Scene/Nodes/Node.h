#pragma once
#include <vector>
#include <memory>
#include "Event/EventHandler.h"
#include "Utilities/PointerHelper.h"

namespace Ull
{
    class Scene;

    class Node : public EventHandler
    {
    public:
        Node(const std::string& name, NotOwner<Scene> scene);
        virtual ~Node();

        void SetName(const std::string& name) { m_name = name; }
        void SetParent(NotOwner<Node> node);
        
        inline bool IsRenerable() const     { return m_isRenderable; }
        NotOwner<Node> GetParent() const    { return m_parent; }
        std::vector<Node*>& GetChildren()   { return m_children; }
        NotOwner<Scene> GetScene() const    { return m_scene; }
        const std::string GetName() const   { return m_name; }

        void AddNode(Node* node);
        bool RemoveNode(Node* node);

        virtual void HandleEvent(Event* event) override;
        virtual void Update() {};

        bool operator==(Node* node);
        bool operator!=(Node* node);

    protected:
        unsigned long long m_id;

        NotOwner<Scene> m_scene{ nullptr };

        NotOwner<Node> m_parent{ nullptr };
        std::vector<Node*> m_children;
        std::string m_name;

        bool m_isRenderable{ false };
    };
}