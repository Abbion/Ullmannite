#include "Ullpch.h"
#include "Scene.h"
#include "Nodes/RenderableNode.h"
#include "Nodes/Node3D.h"
#include <string>

using namespace Ull;

namespace 
{
    Node* DeepSearch(const std::string& name, Node* startNode)
    {
        for (auto& child : startNode->GetChildren())
        {
            if(child->GetName() == name)
                return child;

            auto subChild = DeepSearch(name, child);
            
            if (subChild != nullptr)
                return subChild;
        }
        return nullptr;
    }
}

Scene::Scene(std::string name) :
    m_name(name)
{
    m_root = new Node("3D scene", this);
    m_root->SetParent(nullptr);
}

Scene::~Scene()
{
    delete m_root;
}

Node* Scene::GetNodeByName(const std::string& name) const
{
    return DeepSearch(name, m_root);
}

void Scene::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    m_root->HandleEvent(event);
}

void Scene::Update()
{
    m_mainCamera->Update();
}

void Scene::Render()
{
    for (auto node : m_root->GetChildren())
    {
        //TODO: Put renderable in Node and static_cast
        auto renderNode = dynamic_cast<RenderableNode*>(node);

        if(renderNode != nullptr)
        {
            renderNode->Render();
        }
    }
}
