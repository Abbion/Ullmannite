#include "Ullpch.h"
#include "Scene.h"
#include "Nodes/RenderableNode.h"
#include "Nodes/Node3D.h"
#include <string>

using namespace Ull;

Scene::Scene(std::string name) :
    m_name(name)
{
    m_root = new Node();
    m_root->SetParent(nullptr);
}

Scene::~Scene()
{
    delete m_root;
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
