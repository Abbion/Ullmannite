#pragma once
#include "UiArea.h"
#include "Window/Window.h"

namespace Ull
{
	class UiTitleBar : public UiArea
	{
	public:
		UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size);
		~UiTitleBar() {}

		void CreateResources() override;

		void HandleEvent(Event* event) override;

		void Update() override;
		void Render() override;

		void RenderUI();
	private:

	};
}