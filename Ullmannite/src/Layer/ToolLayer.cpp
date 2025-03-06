#include "Ullpch.h"
#include "ToolLayer.h"

using namespace Ull;

ToolLayer::ToolLayer(const glm::uvec2 size) : Layer("toolLayer", glm::uvec2(0, 0), size, false)
{
    SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    CreateLayout();
}

void ToolLayer::RenderLayerComponents()
{
    UiRenderArea::Render();
}

void ToolLayer::CreateLayout()
{
    const auto initSize = GetSize();

    m_pickerView = std::make_shared<UiPickerView>("colorPicker", glm::uvec2(0, 0));
    m_pickerView->SetPosition((initSize - m_pickerView->GetSize()) / 2.0f);

    AddChildNode(m_pickerView);
}

void ToolLayer::ResizeLayout()
{
    // Do nothing
}