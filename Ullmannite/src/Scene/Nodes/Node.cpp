#include "Ullpch.h"
#include "Node.h"
#include "Logger/Logger.h"
#include "Scene/Scene.h"

using namespace Ull;

Node::Node(const std::shared_ptr<Scene>& scene) :
    m_scene(scene)
{
    static unsigned long long newID = 0;
    m_id = newID++;

    ULOGD("Creeated node with ID" << m_id);
}

Node::~Node()
{
    ULOGD("Removed node with ID" << m_id);
}

void Node::SetParent(Node* node)
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

bool Node::operator==(Node* node)
{
    return this->m_id == node->m_id;
}

bool Node::operator!=(Node* node)
{
    return this->m_id != node->m_id;
}