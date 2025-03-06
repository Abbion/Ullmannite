#include "Ullpch.h"
#include "Application/Application.h"
#include "UiPickerView.h"

using namespace Ull;

namespace {
    constexpr auto viewWidth = 400.0f;
    constexpr auto viewHeight = 300.0f;
}

UiPickerView::UiPickerView(const std::string& name, const glm::uvec2 position) :
    UiRenderArea(name, position, glm::uvec2(viewWidth, viewHeight), false),
    m_frame{ std::make_shared<UiFrame>("pickerViewFrame", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f))},
    m_titleBar{ std::make_shared<UiTitleBar>("pickerViewTitleBar", glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f))},
    m_linearGradient{ std::make_shared<UiLinearColorGradient>("pickerViewLinearGradient", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), UiLinearColorGradient::GradientDirection::VERTICAL) },
    m_linearGradientPick{ std::make_shared<UiSpace>("pickerViewLinearGradientPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_colorGradientRect{ std::make_shared<UiRectGradient>("pickerViewColorGradientRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_horizontalPick{ std::make_shared<UiSpace>("pickerViewHorizontalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_verticalPick{ std::make_shared<UiSpace>("pickerViewVerticalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_selectedColorRect{ std::make_shared<UiSpace>("pickerViewSelectedColorRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) }
{
    SetBackgroundColor(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
    CreateControls();
}

void UiPickerView::HandleEvent(Event* event) 
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::MouseDown:
    {
        const auto mousePosition = Application::GetMouse().GetMousePosition();
        const auto grabArea = m_titleBar->GetGrabArea();

        if (grabArea.IsPointInside(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))
        {
            m_grabStartPosition = mousePosition - glm::ivec2(GetPosition());
            m_isGrabbed = true;
            ULOGD("IN");
        }
    }
    break;

    case EventType::MouseUp:
        m_isGrabbed = false;
        ULOGD("OUT");
    break;

    case EventType::MouseMove:
    {
        if (m_isGrabbed)
        {
            const auto mousePosition = static_cast<MouseMoveEvent*>(event)->GetVal();
            SetPosition(mousePosition - m_grabStartPosition);
        }
    }
    break;
    }

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

void UiPickerView::OnPositionChange()
{
    //m_grabArea = m_titleBar->GetGrabArea();
}

void UiPickerView::CreateControls()
{
    //m_linearGradient->SetPosition(glm::vec2(400.0f, 75.0f));
    //m_linearGradient->SetSize(glm::vec2(200.0f, 50.0f));
    //m_linearGradient->CreateResources();
    //
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.166f,   glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.333f,   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.5f,     glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.666f,   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.833f,   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) });
    //m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 1.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    //m_linearGradient->SetInteractive(true);
    //
    //AddChildNode(m_linearGradient);

    const auto size = GetSize();

    m_frame->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_frame->SetThickness(1.0f);
    m_frame->SetSize(size);
    m_frame->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_frame->CreateResources();
    AddChildNode(m_frame);

    // Title bar
    m_titleBar->SetSize(glm::vec2(size.x - 2.0f, 20.0f));
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::CLOSE, State::Enable);
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::TITLE, State::Enable);
    m_titleBar->SetTitleText(L"Color picker");
    m_titleBar->ResizeControls();

    AddChildNode(m_titleBar);
}