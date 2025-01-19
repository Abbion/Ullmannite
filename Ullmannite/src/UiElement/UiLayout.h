#pragma once
#include "UiElement/UiElement.h"

namespace Ull
{
	class UiLayout : public UiElement
	{
	public:
		UiLayout(const std::string& name, const glm::uvec2 position, const glm::uvec2 size);
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