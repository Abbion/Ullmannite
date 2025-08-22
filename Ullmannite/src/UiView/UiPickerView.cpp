#include "Ullpch.h"
#include "Application/Application.h"
#include "UiPickerView.h"
#include "Layer/ToolLayer.h"
#include "Event/EventAggregator.h"

using namespace Ull;

namespace {
    constexpr auto viewWidth = 400.0f;
    constexpr auto viewHeight = 300.0f;
    constexpr auto MARGIN = 15.0f;
    constexpr auto LINEAR_GRADIENT_PICKER_OVERFLOW = 4.0f;

    constexpr auto PICKER_INACTIVE_COLOR = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
    constexpr auto PICKER_ACTIVE_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    constexpr auto RECT_GRADIENT_PICKER_WIDTH = 2.0f;
}

UiPickerView::UiPickerView(const std::string& name, const glm::uvec2 position, const glm::vec4 startingColor) :
    UiRenderArea(name, position, glm::uvec2(viewWidth, viewHeight), false),
    m_startingColor{ startingColor },
    m_currentColor{ startingColor },
    m_frame{ std::make_shared<UiFrame>("pickerViewFrame", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f))},
    m_titleBar{ std::make_shared<UiTitleBar>("pickerViewTitleBar", glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f))},
    m_linearGradient{ std::make_shared<UiLinearColorGradient>("pickerViewLinearGradient", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), UiLinearColorGradient::GradientDirection::VERTICAL) },
    m_linearGradientPick{ std::make_shared<UiSpace>("pickerViewLinearGradientPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_colorGradientRect{ std::make_shared<UiRectGradient>("pickerViewColorGradientRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_horizontalPick{ std::make_shared<UiSpace>("pickerViewHorizontalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_verticalPick{ std::make_shared<UiSpace>("pickerViewVerticalPick", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_selectedColorRect{ std::make_shared<UiSpace>("pickerViewSelectedColorRect", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_selectedColorRectOutline{ std::make_shared<UiSpace>("pickerViewSelectedColorRectOutline", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_confirmButton{ std::make_shared<UiButton>("pickerViewConfirmButton", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_restoreButton{ std::make_shared<UiButton>("pickerViewRestoreButton", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) },
    m_cancelButton{ std::make_shared<UiButton>("pickerViewCancelButton", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) }
{
    SetBackgroundColor(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
    CreateControls();
    UpdateSelectors();

    RenderAreaSizeChanged renderAreaSizeChangedEvent(EventType::RenderAreaSizeChanged);
    HandleEvent(&renderAreaSizeChangedEvent);
}

void UiPickerView::HandleEvent(Event* event) 
{
    if (event->IsHandeled())
        return;

    const auto mousePositionI = Application::GetMouse().GetMousePosition();
    const auto mousePositionF = glm::vec2(static_cast<float>(mousePositionI.x), static_cast<float>(mousePositionI.y));

    switch (event->GetType())
    {
    case EventType::MouseDown:
    {
        const auto titleGrabArea = m_titleBar->GetGrabArea();

        if (titleGrabArea.IsPointInside(mousePositionF))
        {
            m_grabStartPosition = mousePositionI - glm::ivec2(GetPosition());
            m_isSubWindowGrabbed = true;
        }
        else if (m_linearGradient->IsHover())
        {
            m_isLinearGradientInteracting = true;
            UpdateLinearPickerPosition(mousePositionF);

            m_horizontalPick->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
            m_verticalPick->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
        }
        else if (m_colorGradientRect->IsHover())
        {
            m_isGradientRectInteracting = true;
            UpdateGradientRectPickerPosition(mousePositionF);

            m_linearGradientPick->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
        }
    }
    break;

    case EventType::MouseUp:
        m_isSubWindowGrabbed = false;
        m_isLinearGradientInteracting = false;
        m_isGradientRectInteracting = false;

        m_linearGradientPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
        m_horizontalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
        m_verticalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);

        m_linearGradientPick->SetFunctionality(UiControlFunctionality::Hover, State::Enable);
        m_horizontalPick->SetFunctionality(UiControlFunctionality::Hover, State::Enable);
        m_verticalPick->SetFunctionality(UiControlFunctionality::Hover, State::Enable);

    break;

    case EventType::MouseMove:
    {
        if (m_isSubWindowGrabbed)
        {
            const auto viewSize = GetSize();
            auto newViewPosition = mousePositionI - m_grabStartPosition;

            const auto toolLayer = GetParent();
            const auto layerSize = toolLayer->GetSize();

            if (newViewPosition.x < 1.0f)
                newViewPosition.x = 1.0f;

            else if (newViewPosition.x + viewSize.x > layerSize.x - 1.0f)
                newViewPosition.x = layerSize.x - viewSize.x - 1.0f;

            if (newViewPosition.y < 1.0f)
                newViewPosition.y = 1.0f;

            else if (newViewPosition.y + viewSize.y > layerSize.y - 1.0f)
                newViewPosition.y = layerSize.y - viewSize.y - 1.0f;

            SetPosition(newViewPosition);
        }
        else if (m_isLinearGradientInteracting)
        {
            UpdateLinearPickerPosition(mousePositionF);
        }
        else if (m_isGradientRectInteracting)
        {
            UpdateGradientRectPickerPosition(mousePositionF);
        }
    }
    break;
    }

    UiRenderArea::HandleEvent(event);

    const auto viewGlobalPosition = GetGlobalPosition();
    const auto viewSize = GetSize();
    RectF viewRect(viewGlobalPosition.x, viewGlobalPosition.y, viewSize.x, viewSize.y);

    if (viewRect.IsPointInside(mousePositionF))
        event->MarkHandeled(true);
}

void UiPickerView::Update()
{
    const auto linearGradientPickPosition = m_linearGradientPick->GetPosition() + glm::vec2(0.0f, m_linearGradientPick->GetSize().y / 2.0f);
    const auto pickerRatio = (linearGradientPickPosition.y - m_linearGradient->GetPosition().y) / m_linearGradient->GetSize().y;
    const auto color = m_linearGradient->GetColorForRatio(pickerRatio);
    const auto hslColor = RgbToHsv(color);

    std::vector<UiRectGradient::GradientColorData> hslColorData = { 
                                        UiRectGradient::GradientColorData{ 0, glm::vec3(hslColor.x, 0.0f, 1.0f) },
                                        UiRectGradient::GradientColorData{ 1, glm::vec3(hslColor.x, 1.0f, 1.0f) },
                                        UiRectGradient::GradientColorData{ 2, glm::vec3(hslColor.x, 0.0f, 0.0f) },
                                        UiRectGradient::GradientColorData{ 3, glm::vec3(hslColor.x, 1.0f, 0.0f) } };

    m_colorGradientRect->SetColorData(hslColorData);

    const auto horizontalGradientRatio = (m_horizontalPick->GetPosition().y - m_colorGradientRect->GetPosition().y) / (m_colorGradientRect->GetSize().y - RECT_GRADIENT_PICKER_WIDTH);
    const auto verticalGradientRatio = (m_verticalPick->GetPosition().x - m_colorGradientRect->GetPosition().x) / (m_colorGradientRect->GetSize().x - RECT_GRADIENT_PICKER_WIDTH);

    const auto verticalInterpolationTop = ((1.0f - verticalGradientRatio) * hslColorData[0].color) + (verticalGradientRatio * hslColorData[1].color);
    const auto verticalInterpolationDown = ((1.0f - verticalGradientRatio) * hslColorData[2].color) + (verticalGradientRatio * hslColorData[3].color);
    const auto horizontalInterpolation = ((1.0f - horizontalGradientRatio) * verticalInterpolationTop) + (horizontalGradientRatio * verticalInterpolationDown);
    const auto interpolatedColor = HsvToRgb(horizontalInterpolation);

    m_selectedColorRect->SetBackgroundColor(glm::vec4(interpolatedColor, 1.0f));

    if (!m_isGradientRectInteracting && !m_isLinearGradientInteracting)
    {
        if (m_horizontalPick->IsHover() || m_verticalPick->IsHover())
        {
            m_horizontalPick->SetBackgroundColor(PICKER_ACTIVE_COLOR);
            m_verticalPick->SetBackgroundColor(PICKER_ACTIVE_COLOR);
        }
        else
        {
            m_horizontalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
            m_verticalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
        }
    }

    UiRenderArea::Update();
}

void UiPickerView::Render()
{
    UiRenderArea::Render();
}

void UiPickerView::OnPositionChange()
{
    
}

void UiPickerView::CreateControls()
{
    const auto size = GetSize();

    m_frame->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_frame->SetThickness(1.0f);
    m_frame->SetSize(size);
    m_frame->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_frame->CreateResources();
    AddChildNode(m_frame);

    m_titleBar->SetSize(glm::vec2(size.x - 2.0f, 25.0f));
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::CLOSE, State::Enable);
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::TITLE, State::Enable);
    m_titleBar->SetTitleText(L"Color picker");
    m_titleBar->ResizeControls();
    m_titleBar->SetCloseFunction([this]() {
        m_readyToRelease = true;
    });

    AddChildNode(m_titleBar);

    const auto linearGradientHeight = m_frame->GetSize().y - m_titleBar->GetSize().y - (MARGIN * 2.0f);

    m_linearGradient->SetPosition(glm::vec2(MARGIN, MARGIN + m_titleBar->GetSize().y));
    m_linearGradient->SetSize(glm::vec2(25.0f, linearGradientHeight));
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.166f,   glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.333f,   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.5f,     glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.666f,   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.833f,   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) });
    m_linearGradient->AddColor(UiLinearColorGradient::GradientColorData{ 1.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_linearGradient->SetInteractive(false);
    m_linearGradient->CreateResources();

    AddChildNode(m_linearGradient);

    m_linearGradientPick->SetPosition(m_linearGradient->GetPosition() - glm::vec2(LINEAR_GRADIENT_PICKER_OVERFLOW / 2.0f, 0.0f));
    m_linearGradientPick->SetSize(glm::vec2(m_linearGradient->GetSize().x + LINEAR_GRADIENT_PICKER_OVERFLOW, LINEAR_GRADIENT_PICKER_OVERFLOW));
    m_linearGradientPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
    m_linearGradientPick->SetHoverColor(PICKER_ACTIVE_COLOR);
    m_linearGradientPick->CreateResources();

    AddChildNode(m_linearGradientPick);

    m_colorGradientRect->SetPosition(glm::vec2(m_linearGradient->GetPosition().x + m_linearGradient->GetSize().x + MARGIN, m_linearGradient->GetPosition().y));
    m_colorGradientRect->SetSize(glm::vec2(linearGradientHeight, linearGradientHeight));
    m_colorGradientRect->SetColorData({ UiRectGradient::GradientColorData{ 0, glm::vec3(0.5f, 0.0f, 1.0f) } ,
                                        UiRectGradient::GradientColorData{ 1, glm::vec3(0.5f, 1.0f, 1.0f) },
                                        UiRectGradient::GradientColorData{ 2, glm::vec3(0.5f, 0.0f, 0.0f) },
                                        UiRectGradient::GradientColorData{ 3, glm::vec3(0.5f, 1.0f, 0.0f) } });
    m_colorGradientRect->CreateResources();

    AddChildNode(m_colorGradientRect);

    m_horizontalPick->SetPosition(m_colorGradientRect->GetPosition());
    m_horizontalPick->SetSize(glm::vec2(linearGradientHeight, RECT_GRADIENT_PICKER_WIDTH));
    m_horizontalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
    m_horizontalPick->CreateResources();

    AddChildNode(m_horizontalPick);
    
    m_verticalPick->SetPosition(m_colorGradientRect->GetPosition());
    m_verticalPick->SetSize(glm::vec2(RECT_GRADIENT_PICKER_WIDTH, linearGradientHeight));
    m_verticalPick->SetBackgroundColor(PICKER_INACTIVE_COLOR);
    m_verticalPick->CreateResources();

    AddChildNode(m_verticalPick);

    m_selectedColorRect->SetPosition(m_colorGradientRect->GetPosition() + glm::vec2(m_colorGradientRect->GetSize().x + MARGIN, 2.0f));
    m_selectedColorRect->SetSize(glm::vec2(65.0f, 50.0f));
    m_selectedColorRect->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_selectedColorRect->CreateResources();

    m_selectedColorRectOutline->SetPosition(m_selectedColorRect->GetPosition() - glm::vec2(2, 2));
    m_selectedColorRectOutline->SetSize(m_selectedColorRect->GetSize() + glm::vec2(4, 4));
    m_selectedColorRectOutline->SetBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_selectedColorRectOutline->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_selectedColorRectOutline->CreateResources();

    AddChildNode(m_selectedColorRectOutline);
    AddChildNode(m_selectedColorRect);

    m_confirmButton->SetPosition(m_selectedColorRectOutline->GetPosition() + glm::vec2(0.0f, m_selectedColorRectOutline->GetSize().y + MARGIN / 2.0f));
    m_confirmButton->SetSize(glm::vec2(m_selectedColorRectOutline->GetSize().x, 20.0f));
    m_confirmButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_confirmButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_confirmButton->CreateResources();

    auto confirmButtonTextControl = m_confirmButton->GetTextControl();
    confirmButtonTextControl->SetFontSize(13.0f);
    confirmButtonTextControl->SetString(std::wstring(L"OK"));
    confirmButtonTextControl->SetEdgeSmoothing(2.5f);
    confirmButtonTextControl->SetSampleThreshold(2.0f);

    AddChildNode(m_confirmButton);

    m_restoreButton->SetPosition(m_confirmButton->GetPosition() + glm::vec2(0.0f, m_confirmButton->GetSize().y + MARGIN / 2.0f));
    m_restoreButton->SetSize(glm::vec2(m_selectedColorRectOutline->GetSize().x, 20.0f));
    m_restoreButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_restoreButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_restoreButton->CreateResources();

    m_restoreButton->SetOnClickFunction([this](UiButton& buttonElement) {
        m_currentColor = m_startingColor;
        UpdateSelectors();
    });

    auto restoreButtonTextControl = m_restoreButton->GetTextControl();
    restoreButtonTextControl->SetFontSize(13.0f);
    restoreButtonTextControl->SetString(std::wstring(L"Restore"));
    restoreButtonTextControl->SetEdgeSmoothing(2.5f);
    restoreButtonTextControl->SetSampleThreshold(2.0f);

    AddChildNode(m_restoreButton);

    m_cancelButton->SetPosition(m_restoreButton->GetPosition() + glm::vec2(0.0f, m_restoreButton->GetSize().y + MARGIN / 2.0f));
    m_cancelButton->SetSize(glm::vec2(m_selectedColorRectOutline->GetSize().x, 20.0f));
    m_cancelButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_cancelButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_cancelButton->CreateResources();

    m_cancelButton->SetOnClickFunction([this](UiButton& buttonElement) {
        m_currentColor = m_startingColor;
        m_readyToRelease = true;
        });

    auto cancelButtonTextControl = m_cancelButton->GetTextControl();
    cancelButtonTextControl->SetFontSize(13.0f);
    cancelButtonTextControl->SetString(std::wstring(L"Cancel"));
    cancelButtonTextControl->SetEdgeSmoothing(2.5f);
    cancelButtonTextControl->SetSampleThreshold(2.0f);
    cancelButtonTextControl->SetSmoothingExceptance({ 'l' });

    AddChildNode(m_cancelButton);
}

void UiPickerView::UpdateLinearPickerPosition(const glm::vec2 cursorPosition)
{
    const auto inLinearGradientPosition = cursorPosition - m_linearGradient->GetGlobalPosition();
    const auto halfPickSize = m_linearGradientPick->GetSize().y / 2.0f;
    
    auto newPickerPosition = m_linearGradient->GetPosition() +
        glm::vec2(-LINEAR_GRADIENT_PICKER_OVERFLOW / 2.0f, inLinearGradientPosition.y - halfPickSize);

    newPickerPosition.y = max(newPickerPosition.y, m_linearGradient->GetPosition().y - halfPickSize);
    newPickerPosition.y = min(newPickerPosition.y, m_linearGradient->GetPosition().y + m_linearGradient->GetSize().y - halfPickSize);

    m_linearGradientPick->SetPosition(newPickerPosition);
    m_linearGradientPick->SetBackgroundColor(PICKER_ACTIVE_COLOR);
}

void UiPickerView::UpdateGradientRectPickerPosition(const glm::vec2 cursorPosition)
{
    const auto inGradientRectPosition = cursorPosition - m_colorGradientRect->GetGlobalPosition();
    auto newPickerPosition = m_colorGradientRect->GetPosition() + inGradientRectPosition;

    newPickerPosition.x = min(newPickerPosition.x, m_colorGradientRect->GetPosition().x + m_colorGradientRect->GetSize().x - RECT_GRADIENT_PICKER_WIDTH);
    newPickerPosition.x = max(newPickerPosition.x, m_colorGradientRect->GetPosition().x);
    newPickerPosition.y = min(newPickerPosition.y, m_colorGradientRect->GetPosition().y + m_colorGradientRect->GetSize().y - RECT_GRADIENT_PICKER_WIDTH);
    newPickerPosition.y = max(newPickerPosition.y, m_colorGradientRect->GetPosition().y);

    m_horizontalPick->SetPosition(glm::vec2(m_horizontalPick->GetPosition().x, newPickerPosition.y));
    m_verticalPick->SetPosition(glm::vec2(newPickerPosition.x, m_verticalPick->GetPosition().y));

    m_horizontalPick->SetBackgroundColor(PICKER_ACTIVE_COLOR);
    m_verticalPick->SetBackgroundColor(PICKER_ACTIVE_COLOR);
}

void UiPickerView::UpdateSelectors() 
{
    const auto hsvColor = RgbToHsv(glm::vec3(m_currentColor.x, m_currentColor.y, m_currentColor.z));
    const auto hueNormalized = hsvColor.r / 360.f;
    const auto hueOffset = hueNormalized * m_linearGradient->GetSize().y;
    const auto linearGradientPickStartPosition = m_linearGradient->GetPosition().y - (m_linearGradientPick->GetSize().y / 2.0f);

    m_linearGradientPick->SetPosition(glm::vec2(m_linearGradientPick->GetPosition().x, linearGradientPickStartPosition + hueOffset));

    const auto saturation = hsvColor.g;
    m_verticalPick->SetPosition(m_colorGradientRect->GetPosition() + glm::vec2(m_colorGradientRect->GetSize().x * saturation - 2.0f, 0.0f));

    const auto brightness = hsvColor.b;
    m_horizontalPick->SetPosition(m_colorGradientRect->GetPosition() + glm::vec2(0.0f, m_colorGradientRect->GetSize().y * (1.0f - brightness)));
}