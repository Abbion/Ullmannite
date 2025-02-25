#include "Ullpch.h"
#include "ToolLayer.h"

using namespace Ull;

//ToolLayer::ToolLayer(const glm::uvec2 size) : 
//    Layer("ToolLayer", size),
//    m_pickerView{ std::make_shared<UiPickerView>("ToolLayerColorPicker", glm::uvec2(50.0f, 50.0f)) }
//{
//    CreateLayout();
//}
//
//void ToolLayer::HandleEvent(Event* event)
//{
//    m_layout->HandleEvent(event);
//}
//
//void ToolLayer::Update()
//{
//    for (auto& element : m_layout->GetChildren())
//        element->Update();
//}
//
//void ToolLayer::Render()
//{
//    m_layout->Render();
//}
//
//void ToolLayer::CreateLayout()
//{
//    m_layout->AddChildNode(m_pickerView);
//}
//
//void ToolLayer::Resize(const glm::uvec2 size)
//{
//
//}