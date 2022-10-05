#pragma once
#include "UiElement/UiElement.h"
#include "Rendering/Drawable/Drawable.h"

namespace Ull
{
	class UiLayout : public UiElement, DrawableInterface
	{
	public:
		UiLayout(std::string name, glm::uvec2 position, glm::uvec2 size);
		~UiLayout();

		void CreateResources() override;
		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

	private:
		void CreateRenderAreaForUiElements(unsigned int elementID);

		glm::mat4 m_viewMatrix;
	};
}