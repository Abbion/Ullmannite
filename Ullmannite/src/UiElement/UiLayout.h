#pragma once
#include "UiElement/UiElement.h"

namespace Ull
{
	class UiLayout : public UiElement
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

		VertexBuffer* m_vertexBuffer{ nullptr };
		IndexBuffer* m_indexBuffer{ nullptr };
		VertexLayout* m_layout{ nullptr };
		Shader* m_shader{ nullptr };
	};
}