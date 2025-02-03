#include "Ullpch.h"
#include "UiPickerView.h"

using namespace Ull;

namespace {
    constexpr auto viewWidth = 450.0f;
    constexpr auto viewHeight = 350.0f;
}

UiPickerView::UiPickerView(const std::string& name, const glm::uvec2 position) :
    UiRenderArea(name, position, glm::uvec2(viewWidth, viewHeight), false),
    m_linearGradient{ std::make_shared<UiLinearColorGradient>("pickerViewLinearGradient", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), UiLinearColorGradient::GradientDirection::VERTICAL) },
    m_linearGradientPick{ std::make_shared<UiSpace>("pickerViewLinearGradientPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_colorGradientRect{ std::make_shared<UiRectGradient>("pickerViewColorGradientRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_horizontalPick{ std::make_shared<UiSpace>("pickerViewHorizontalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_verticalPick{ std::make_shared<UiSpace>("pickerViewVerticalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_selectedColorRect{ std::make_shared<UiSpace>("pickerViewSelectedColorRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) }
{
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.166f,   glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.333f,   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.5f,     glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.666f,   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.833f,   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 1.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_linearGradient->SetInteractive(true);
}

void UiPickerView::HandleEvent(Event* event) 
{
    UiRenderArea::HandleEvent(event);
}

void UiPickerView::Update()
{
    UiRenderArea::Update();
}

void UiPickerView::Render()
{
    UiRenderArea::Render();
}

void UiPickerView::CreateControls()
{
    m_linearGradient->SetPosition(glm::vec2(400.0f, 75.0f));
    m_linearGradient->SetSize(glm::vec2(200.0f, 50.0f));
    m_linearGradient->CreateResources();
    AddChildNode(m_linearGradient);
}