#include "Ullpch.h"
#include "UiLayout.h"
#include "Application/Application.h"
#include "UiRenderArea.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Ull;

UiLayout::UiLayout(const std::string& name, const glm::uvec2 size) :
	UiElement(name, glm::uvec2(0, 0), size, UiElementType::Layout)
{
    auto& shaderManager = Application::GetRenderer().GetShaderManager();
	m_shader = shaderManager.GetShader(ShaderTag::FRAME_DISPLAY_SHADER);
	CreateResources();
}

UiLayout::~UiLayout()
{

}

void UiLayout::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    for (const auto& uiElement : GetChildren())
        uiElement->HandleEvent(event);
}

void UiLayout::Render()
{
    for (const auto& element : GetChildren())
    {
        if (element->GetType() != UiElementType::RenderArea)
        {
            ULOGE(GetName() << " has a child element named: " << element->GetName() << " that is not a RenderArea!");
            continue;
        }

        auto renderAreaElement = static_cast<UiRenderArea*>(element.get());
        renderAreaElement->Render();
        m_frameBufferRenderer.Render(*renderAreaElement);
    }
}