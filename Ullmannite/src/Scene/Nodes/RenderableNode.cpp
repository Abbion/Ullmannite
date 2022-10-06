#include "Ullpch.h"
#include "RenderableNode.h"

using namespace Ull;

RenderableNode::RenderableNode()
{

}

void RenderableNode::Render()
{
    for (auto node : GetChildren())
    {
        //TODO: Put renderable in Node and static_cast
        auto renderNode = dynamic_cast<Renderable*>(node);

        if (renderNode != nullptr)
        {
            renderNode->Render();
        }
    }
}