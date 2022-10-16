#include "Ullpch.h"
#include "RenderableNode.h"

using namespace Ull;

RenderableNode::RenderableNode(const std::string& name, NotOwner<Scene> scene) : 
    Node(name, scene)
{
    m_isRenderable = true;
}

void RenderableNode::Render()
{
    for (auto node : GetChildren())
    {
        if (node->IsRenerable())
        {
            auto renderNode = static_cast<RenderableNode*>(node);
            renderNode->Render();
        }
    }
}