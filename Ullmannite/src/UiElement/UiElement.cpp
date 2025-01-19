#include "Ullpch.h"
#include "UiElement.h"
#include <glm/ext/matrix_transform.hpp>
#include <limits.h>

namespace
{
	constexpr auto EPSILON = std::numeric_limits<float>::epsilon();
}

using namespace Ull;

UiElement::UiElement(const std::string& name, const glm::vec2 position, const glm::vec2 size, const UiElementType type) :
	TreeNode{ name },
	Object2D{ position, size },
	m_uiElementType{ type }
{
	if (size.x < EPSILON || size.y < EPSILON)
		ULOGE("UiElement error: " << name << " size cannot be 0. size x: " << size.x << " size.y: " << size.y);
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

glm::vec2 UiElement::GetGlobalPosition() const 
{
	auto parent = GetParent();
	auto position = GetPosition();

	while (parent != nullptr)
	{
		position += parent->GetPosition();
		parent = parent->GetParent();
	}

	return position;
}

glm::vec2 UiElement::GetRenderAreaPosition() const 
{
	auto parent = GetParent();
	auto position = GetPosition();

	while (parent != nullptr)
	{
		if(parent->GetType() != UiElementType::RenderArea)
		{
			position += parent->GetPosition();
			return position;
		}

		parent = parent->GetParent();
	}

	return position;
}

glm::mat4 UiElement::GetTransform() const
{
	auto parent = GetParent();
	auto position = GetRenderAreaPosition();
	auto size = GetSize();

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
