#include "Ullpch.h"
#include "ToolLayer.h"
#include "Event/EventAggregator.h"

using namespace Ull;

ToolLayer::ToolLayer(const glm::uvec2 size, const NotOwner<LayerManager>& layerManager) : Layer(LayerNames::toolLayer, glm::uvec2(0, 0), size, false, layerManager)
{
    SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
    CreateLayout();
    RegisterToolCreationEvents();
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

void ToolLayer::SetWindow(const NotOwner<UllWindow>& window)
{
    m_window = window;
}

void ToolLayer::RenderLayerComponents()
{
    UiRenderArea::Render();
}

void ToolLayer::RegisterToolCreationEvents()
{
    EventAggregator::Sbuscribe(EventType::OpenTool, [this](const std::shared_ptr<Event>& openEvent) {
        if (m_pickerView != nullptr)
            return;

        const auto openToolEvent = dynamic_cast<OpenToolEvent*>(openEvent.get());
        UASSERT(openToolEvent != nullptr, "Open event failed to convert!");
        const auto& toolData = openToolEvent->GetVal();

        if (toolData.toolType == ToolType::ColorPicker)
        {
            const auto colorPickerData = std::get<ColorPickerData>(toolData.toolData);
            m_pickerView = std::make_shared<UiPickerView>("colorPicker", toolData.spawnPoint, colorPickerData.initialColor, std::move(colorPickerData.onColorChange));
            AddChildNode(m_pickerView);
        }
    });
}

void ToolLayer::CreateLayout()
{

}

void ToolLayer::ResizeLayout()
{
    //Do nothing
}