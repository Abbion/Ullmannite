#pragma once
#include "TreeNode.h"
#include "Event/EventHandler.h"

namespace Ull
{
	class ReactiveNode;
	typedef std::shared_ptr<ReactiveNode> ReactiveNodeRef;

	class ReactiveNode : public TreeNode, public EventHandler
	{
	public:
		using TreeNode::TreeNode;

		void AddChildNode(ReactiveNodeRef childNode);
		virtual void HandleEvent(Event* event);

	private:

		// Deleted functions
		void AddChildElement(TreeNodeRef childNode) {};
	};
}