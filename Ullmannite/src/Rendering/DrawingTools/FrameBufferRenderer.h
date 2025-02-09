#pragma once
#include "Rendering/DrawInterfaces/Drawable.h"
#include <glm/glm.hpp>
#include "UiElement/UiRenderArea.h"

namespace Ull
{
	class FrameBufferRenderer : private Drawable
	{
	public:
		FrameBufferRenderer();
		virtual ~FrameBufferRenderer() {};

		void Render(UiRenderArea& uiRenderArea);

	private:
		virtual void Render();

		void CreateRenderArea();

		glm::vec2 m_position{ 0.0f, 0.0f };
		glm::vec2 m_size{ 1.0f, 1.0f };
		glm::mat4 m_viewMatrix;
	};
}