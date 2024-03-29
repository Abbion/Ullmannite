#include "Ullpch.h"
#include "Node.h"
#include "Logger/Logger.h"
#include "Scene/Scene.h"

using namespace Ull;

Node::Node(const std::string& name, NotOwner<Scene> scene) :
    m_scene(scene),
    m_name(name)
{
    static unsigned long long newID = 0;
    m_id = newID++;

    ULOGD("Creeated node with ID " << m_id << " name " << m_name);
}

Node::~Node()
{
    for (auto node : m_children)
        delete node;

    ULOGD("Removed node with ID " << m_id << " name " << m_name);
}

void Node::SetParent(NotOwner<Node> node)
{
    m_parent = node;
}

void Node::AddNode(Node* node)
{
    m_children.push_back(std::move(node));
}

bool Node::RemoveNode(Node* node)
{
    for(auto itr = m_children.begin(); itr != m_children.end(); itr++)
    {
        if((*itr) == node)
        {
            m_children.erase(itr);
            return true;
        }
    }

    return false;
}

void Node::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    for(auto node : m_children)
        node->HandleEvent(event);
}

bool Node::operator==(Node* node)
{
    return this->m_id == node->m_id;
}

bool Node::operator!=(Node* node)
{
    return this->m_id != node->m_id;
}