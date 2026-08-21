#include "Ullpch.h"
#include "UiSceneView.h"

using namespace Ull;

UiSceneView::UiSceneView(const std::string& name) :
	UiElement(name, { 0.0f, 0.0f }, { 1.0f, 1.0f }, UiElementType::ScenView),
	m_scene(name + " scene tree")
{
}

void UiSceneView::HandleEvent(Event* event)
{
	m_scene.HandleEvent(event);
}

void UiSceneView::Update()
{
	m_scene.Update();
}

void UiSceneView::Render()
{
	m_scene.Render();
}
