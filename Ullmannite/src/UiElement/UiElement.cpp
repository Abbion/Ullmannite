#include "Ullpch.h"
#include "UiElement.h"
#include <glm/ext/matrix_transform.hpp>

using namespace Ull;

UiElement::UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size, const UiElementType type) :
	TreeNode{ name },
	Object2D{ position, size },
	m_uiElementType{ type }
{
}

std::optional<NotOwner<UiElement>> UiElement::FindUiElementAboveByType(const UiElementType type)
{
	auto parent = GetParent();

	while (parent.Get() != nullptr)
	{
		if (parent->m_uiElementType == type)
			return parent;

		parent = parent->GetParent();
	}

	return std::nullopt;
}

glm::mat4 UiElement::GetTransform() const
{
	auto parent = GetParent();
	auto position = GetPosition();
	const auto size = GetSize();

	while (parent != nullptr)
	{
		position += parent->GetPosition();
		parent = parent->GetParent();
	}

	auto transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3{ position.x, position.y, 0.0f });
	transform = glm::scale(transform, glm::vec3{ size.x, size.y, 1.0f });

	return transform;
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
