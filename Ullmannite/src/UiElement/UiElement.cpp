#include "Ullpch.h"
#include "UiElement.h"

using namespace Ull;

void UiElement::AddChildNode(UiElementRef childNode)
{
	TreeNode::AddChildNode(childNode);
}

std::vector<UiElementRef>& UiElement::GetChildren()
{
	return reinterpret_cast<std::vector<UiElementRef>&>(TreeNode::GetChildren());
}

void UiElement::HandleEvent(Event* event)
{
	event->IsHandeled();

	for (auto& child : GetChildren())
		static_cast<UiElement*>(child.get())->HandleEvent(event);
}

void UiElement::Update()
{
	for (auto& child : GetChildren())
		static_cast<UiElement*>(child.get())->Update();
}

void UiElement::Render()
{
	for (auto& child : GetChildren())
		static_cast<UiElement*>(child.get())->Render();
}
