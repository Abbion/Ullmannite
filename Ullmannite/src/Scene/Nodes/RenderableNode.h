#pragma once
#include "Node.h"
#include "Rendering/DrawInterfaces/Renderable.h"
#include "Utilities/PointerHelper.h"

namespace Ull
{
	class RenderableNode : public Node, public Renderable
	{
	public:
		RenderableNode(const std::string& name, NotOwner<Scene> scene);

		void Render() override;
	};
}