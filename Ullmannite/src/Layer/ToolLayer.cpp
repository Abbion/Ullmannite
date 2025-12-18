#include "Ullpch.h"
#include "ToolLayer.h"

using namespace Ull;

ToolLayer::ToolLayer(const glm::uvec2 size, const NotOwner<LayerManager>& layerManager) : Layer(LayerNames::toolLayer, glm::uvec2(0, 0), size, false, layerManager)
{
    SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    CreateLayout();
}

void ToolLayer::Update()
{
    if (m_pickerView != nullptr && m_pickerView->IsReadyToRelease())
    {
        RemoveChildNode(m_pickerView);
        m_pickerView.reset();
    }

    Layer::Update();
}

void ToolLayer::HandleEvent(Event* event)
{
    if (event->GetType() == EventType::OpenTool)
    {
        if (m_pickerView != nullptr)
            return;

        const auto openToolEvent = dynamic_cast<OpenToolEvent*>(event);
        UASSERT(openToolEvent != nullptr, "Open event failed to convert!");
        const auto& toolData = openToolEvent->GetVal();

        if (toolData.toolType == ToolType::ColorPicker)
        {
            const auto colorPickerData = std::get<ColorPickerData>(toolData.toolData);
            m_pickerView = std::make_shared<UiPickerView>("colorPicker", toolData.spawnPoint, colorPickerData.initialColor, std::move(colorPickerData.onColorChange));
            AddChildNode(m_pickerView);
        }

        event->MarkHandeled(true);
        return;
    }

    Layer::HandleEvent(event);
}

void ToolLayer::SetWindow(const NotOwner<UllWindow>& window)
{
    m_window = window;
}

void ToolLayer::RenderLayerComponents()
{
    UiRenderArea::Render();
}

void ToolLayer::CreateLayout()
{

}

void ToolLayer::ResizeLayout()
{
    //Do nothing
}