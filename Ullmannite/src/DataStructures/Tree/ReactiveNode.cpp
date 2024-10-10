#include "Ullpch.h"
#include "ReactiveNode.h"

using namespace Ull;

void ReactiveNode::AddChildNode(ReactiveNodeRef childNode)
{
	TreeNode::AddChildNode(childNode);
}

void ReactiveNode::HandleEvent(Event* event)
{
	if (event->IsHandeled())
		return;

	for (auto& child : GetChildren())
		static_cast<ReactiveNode*>(child.get())->HandleEvent(event);
}
