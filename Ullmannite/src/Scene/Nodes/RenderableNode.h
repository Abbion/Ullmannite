#pragma once
#include "Node.h"
#include "Rendering/DrawInterfaces/Renderable.h"

namespace Ull
{
	class RenderableNode : public Node, public Renderable
	{
	public:
		RenderableNode();

		void Render() override;
	};
}