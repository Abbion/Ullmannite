#include "Ullpch.h"
#include "UiTwoSideSlider.h"
#include "Application/Application.h"

using namespace Ull;

namespace 
{
    constexpr auto SLIDER_WIDTH = 8.0f;
    constexpr auto SLIDER_INACTIVE_COLOR = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    constexpr auto SLIDER_ACTIVE_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

UiTwoSideSlider::UiTwoSideSlider(const std::string& name, const glm::vec2 position, const glm::vec2 size, const float limitMinValue, const float limitMaxValue) :
    m_limitMinValue{ limitMinValue },
    m_limitMaxValue{ limitMaxValue },
	UiBasicControl(name, position, size, UiControlType::UiTwoWaySlider),
	m_minSlider{ std::make_shared<UiSpace>(name + "MinSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, size.y)) },
	m_maxSlider{ std::make_shared<UiSpace>(name + "MaxSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, size.y)) },
    m_mainSlider{ std::make_shared<UiSpace>(name + "MainSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, size.y)) },
    m_minSliderValueText{ std::make_shared<UiText>(name + "MinText", glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 10.0f), L"-1") },
    m_maxSliderValueText{ std::make_shared<UiText>(name + "MaxText", glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 10.0f), L"1") }
{
	CreateControls();
    UpdateSliderValues();
}

float UiTwoSideSlider::GetMinSliderValue() const
{
    return 0.0f;
}

float UiTwoSideSlider::GetMaxSliderValue() const
{
    return 0.0f;
}

void UiTwoSideSlider::SetSize(const glm::vec2 size)
{
    Object2D::SetSize(size);
    CreateControls();
}

void UiTwoSideSlider::SetMinValue(const float min)
{
    if (min > m_limitMaxValue or min > m_maxValue)
    {
        ULOGE("New min value greather than max limit or current max value");
        return;
    }

    m_minValue = min;

    const auto range = std::abs(m_limitMinValue) + std::abs(m_limitMaxValue);
    const auto rate = std::abs(m_limitMinValue) + m_minValue;
    const auto precentage = rate / range;
    const auto sliderPositionX = (GetSize().x - (SLIDER_WIDTH * 2.0f)) * precentage;
    
    const auto newPosition = glm::vec2(sliderPositionX, 0.0f);
    m_minSlider->SetPosition(newPosition);
    m_mainSlider->SetPosition(newPosition + glm::vec2(SLIDER_WIDTH, 0.0f));
    m_mainSlider->SetSize(glm::vec2(m_maxSlider->GetPosition().x - m_minSlider->GetPosition().x - SLIDER_WIDTH, GetSize().y));

    m_maxDragBounderies.x = m_minSlider->GetPosition().x + m_minSlider->GetSize().x;
    UpdateSliderValues();
}

void UiTwoSideSlider::SetMaxValue(const float max)
{
    if (max < m_limitMinValue or max < m_minValue)
    {
        ULOGE("New max value lesser than min limit or current min value");
        return;
    }

    m_maxValue = max;

    const auto range = std::abs(m_limitMinValue) + std::abs(m_limitMaxValue);
    const auto rate = std::abs(m_limitMinValue) + m_maxValue;
    const auto precentage = rate / range;
    const auto sliderPositionX = (GetSize().x - (SLIDER_WIDTH * 2.0f)) * precentage;

    const auto newPosition = glm::vec2(sliderPositionX + SLIDER_WIDTH, 0.0f);
    m_maxSlider->SetPosition(newPosition);
    m_mainSlider->SetSize(glm::vec2(m_maxSlider->GetPosition().x - m_minSlider->GetPosition().x - SLIDER_WIDTH, GetSize().y));

    m_minDragBounderies.y = m_maxSlider->GetPosition().x - m_maxSlider->GetSize().x;
    UpdateSliderValues();
}

void UiTwoSideSlider::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::MouseMove:
    {
        const auto currentMousePositon = Application::GetMouse().GetMousePosition();

        if (m_activeHandle == UiTwoSideSliderHandle::MIN)
        {
            const auto dragDirection = currentMousePositon - (m_grabPoint + glm::ivec2(m_minSlider->GetGlobalPosition()));
            auto newPosition = m_minSlider->GetPosition() + glm::vec2(dragDirection.x, 0.0f);
            newPosition.x = std::clamp(newPosition.x, m_minDragBounderies.x, m_minDragBounderies.y);
            m_minSlider->SetPosition(newPosition);
            m_mainSlider->SetPosition(newPosition + glm::vec2(SLIDER_WIDTH, 0.0f));
            m_mainSlider->SetSize(glm::vec2(m_maxSlider->GetPosition().x - m_minSlider->GetPosition().x - SLIDER_WIDTH, GetSize().y));

            m_maxDragBounderies.x = m_minSlider->GetPosition().x + m_minSlider->GetSize().x;
            UpdateSliderValues();
            
        }
        else if (m_activeHandle == UiTwoSideSliderHandle::MAX)
        {
            const auto dragDirection = currentMousePositon - (m_grabPoint + glm::ivec2(m_maxSlider->GetGlobalPosition()));
            auto newPosition = m_maxSlider->GetPosition() + glm::vec2(dragDirection.x, 0.0f);
            newPosition.x = std::clamp(newPosition.x, m_maxDragBounderies.x, m_maxDragBounderies.y);
            m_maxSlider->SetPosition(newPosition);
            m_mainSlider->SetSize(glm::vec2(m_maxSlider->GetPosition().x - m_minSlider->GetPosition().x - SLIDER_WIDTH, GetSize().y));

            m_minDragBounderies.y = m_maxSlider->GetPosition().x - m_maxSlider->GetSize().x;
            UpdateSliderValues();
        }
        else if (m_activeHandle == UiTwoSideSliderHandle::MAIN)
        {
            const auto dragDirection = currentMousePositon - (m_grabPoint + glm::ivec2(m_mainSlider->GetGlobalPosition()));
            auto newPosition = m_mainSlider->GetPosition() + glm::vec2(dragDirection.x, 0.0f);

            newPosition.x = std::clamp(newPosition.x, SLIDER_WIDTH, GetSize().x - SLIDER_WIDTH - m_mainSlider->GetSize().x);
            m_mainSlider->SetPosition(newPosition);

            m_minSlider->SetPosition(m_mainSlider->GetPosition() - glm::vec2(SLIDER_WIDTH, 0.0f));
            m_maxSlider->SetPosition(m_mainSlider->GetPosition() + glm::vec2(m_mainSlider->GetSize().x, 0.0f));

            m_maxDragBounderies.x = m_minSlider->GetPosition().x + m_minSlider->GetSize().x;
            m_minDragBounderies.y = m_maxSlider->GetPosition().x - m_maxSlider->GetSize().x;
            UpdateSliderValues();
        }
    }
    break;

    case EventType::MouseDown:
        if (m_minSlider->IsHover())
        {
            m_grabPoint = Application::GetMouse().GetMousePosition() - glm::ivec2(m_minSlider->GetGlobalPosition());
            m_activeHandle = UiTwoSideSliderHandle::MIN;
            m_minSlider->SetBackgroundColor(SLIDER_ACTIVE_COLOR);
        }
        else if (m_maxSlider->IsHover())
        {
            m_grabPoint = Application::GetMouse().GetMousePosition() - glm::ivec2(m_maxSlider->GetGlobalPosition());
            m_activeHandle = UiTwoSideSliderHandle::MAX;
            m_maxSlider->SetBackgroundColor(SLIDER_ACTIVE_COLOR);
        }
        else if (m_mainSlider->IsHover())
        {
            m_grabPoint = Application::GetMouse().GetMousePosition() - glm::ivec2(m_mainSlider->GetGlobalPosition());
            m_activeHandle = UiTwoSideSliderHandle::MAIN;

            m_mainSliderBaseColor = m_mainSlider->GetBackgroundColor();
            m_mainSlider->SetBackgroundColor(SLIDER_ACTIVE_COLOR);
            m_mainSliderBaseColorSampled = true;
        }
        else
        {
            m_activeHandle = UiTwoSideSliderHandle::NONE;
        }
    break;

    case EventType::MouseUp:
    case EventType::WindowLostFocus:
    {
        m_activeHandle = UiTwoSideSliderHandle::NONE;
        m_minSlider->SetBackgroundColor(SLIDER_INACTIVE_COLOR);
        m_maxSlider->SetBackgroundColor(SLIDER_INACTIVE_COLOR);

        if (m_mainSliderBaseColorSampled == true)
        {
            m_mainSlider->SetBackgroundColor(m_mainSliderBaseColor);
            m_mainSliderBaseColorSampled = false;
        }
    }
    break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiTwoSideSlider::Update()
{
    UiBasicControl::Update();
}

void UiTwoSideSlider::CreateControls()
{
    SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    SetFunctionality(UiControlFunctionality::Hover, State::Disable);

    const auto sliderSize = GetSize();

    m_mainSlider->SetPosition(glm::vec2(SLIDER_WIDTH, 0.0f));
    m_mainSlider->SetSize(glm::vec2(sliderSize.x - (SLIDER_WIDTH * 2.0f), sliderSize.y));
    m_mainSlider->CreateResources();

    AddChildNode(m_mainSlider);

    m_minSlider->SetSize(glm::vec2(SLIDER_WIDTH, sliderSize.y));
    m_minSlider->CreateResources();
    m_minSlider->SetBackgroundColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    m_minDragBounderies.x = 0.0f;
    m_minDragBounderies.y = sliderSize.x - (SLIDER_WIDTH * 2.0f);

    AddChildNode(m_minSlider);

    m_maxSlider->SetSize(glm::vec2(SLIDER_WIDTH, sliderSize.y));
    m_maxSlider->SetPosition(glm::vec2(sliderSize.x - m_maxSlider->GetSize().x, 0.0f));
    m_maxSlider->CreateResources();
    m_maxSlider->SetBackgroundColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    m_maxDragBounderies.x = SLIDER_WIDTH;
    m_maxDragBounderies.y = sliderSize.x - SLIDER_WIDTH;

    AddChildNode(m_maxSlider);

    m_minSliderValueText->SetPosition(glm::vec2(0, 30));
    m_minSliderValueText->SetSize(glm::vec2(GetSize().x / 2.0f, 20));
    m_minSliderValueText->SetFontSize(14);
    m_minSliderValueText->SetEdgeSmoothing(3.5f);
    m_minSliderValueText->SetSampleThreshold(1.0f);
    m_minSliderValueText->SetSmoothingExceptance({ L'-' });
    m_minSliderValueText->CreateResources();

    AddChildNode(m_minSliderValueText);

    m_maxSliderValueText->SetPosition(glm::vec2(GetSize().x / 2.0f, 30));
    m_maxSliderValueText->SetSize(glm::vec2(GetSize().x / 2.0f, 20));
    m_maxSliderValueText->SetFontSize(14);
    m_maxSliderValueText->SetEdgeSmoothing(3.5f);
    m_maxSliderValueText->SetSampleThreshold(1.0f);
    m_maxSliderValueText->SetSmoothingExceptance({ L'-' });
    m_maxSliderValueText->SetAlignment(UiText::HorizontalAlignment::RIGHT, UiText::VerticalAlignment::CENTER);
    m_maxSliderValueText->CreateResources();

    AddChildNode(m_maxSliderValueText);
}

void UiTwoSideSlider::UpdateSliderValues()
{
    const auto minValuePickPosition = m_minSlider->GetPosition().x;
    const auto valueRange = GetSize().x - (SLIDER_WIDTH * 2.0f);

    const auto minPercentage = minValuePickPosition / valueRange;
    const auto minValue = ((1.0f - minPercentage) * m_limitMinValue) + (minPercentage * m_limitMaxValue);
    
    const auto maxValuePickerPosition = m_maxSlider->GetPosition().x - SLIDER_WIDTH;
    const auto maxPercentage = maxValuePickerPosition / valueRange;
    auto a = (1.0f - maxPercentage);
    auto b = a * m_limitMinValue;
    auto c = maxPercentage * m_limitMaxValue;

    const auto maxValue = ((1.0f - maxPercentage) * m_limitMinValue) + (maxPercentage * m_limitMaxValue);

    m_minValue = minValue;
    m_maxValue = maxValue;

    m_minSliderValueText->SetString(std::format(L"{:.2f}", m_minValue));
    m_maxSliderValueText->SetString(std::format(L"{:.2f}", m_maxValue));
}