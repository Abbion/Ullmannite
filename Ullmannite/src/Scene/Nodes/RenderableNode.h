#pragma once
#include "Node.h"
#include "Rendering/DrawInterfaces/Renderable.h"

namespace Ull
{
	class RenderableNode : public Node, public Renderable
	{
	public:
		RenderableNode(const std::string& name, const Scene* scene) : Node(name, scene) {}

		void Render() override;
	};
}