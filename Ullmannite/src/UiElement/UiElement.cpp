#include "Ullpch.h"
#include "UiElement.h"

using namespace Ull;

UiElement::UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size) :
	TreeNode{ name },
	Object2D{ position, size }
{
}

void UiElement::HandleEvent(Event* event)
{
	event->IsHandeled();

	for (auto& child : GetChildren())
		child->HandleEvent(event);
}

void UiElement::Update()
{
	for (auto& child : GetChildren())
		child->Update();
}

void UiElement::Render()
{
	for (auto& child : GetChildren())
		child->Render();
}
