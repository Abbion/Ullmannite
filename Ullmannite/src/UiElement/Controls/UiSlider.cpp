#include "Ullpch.h"
#include "UiSlider.h"
#include "Rendering/Api/Renderer.h"
#include "Application/Application.h"

using namespace Ull;

UiSlider::UiSlider(std::string name, glm::vec2 position, glm::vec2 size) :
    UiBasicControl(name, position, size, UiControlType::UiSlider),
    m_sliderText{ std::make_shared<UiText>(std::string(name + "Text"), glm::vec2(0.0f, 0.0f), size, L"0.0") },
    m_sliderHandle{ std::make_shared<UiSpace>(std::string(name + "Handle"), glm::vec2(0.0f, 0.0f), glm::vec2(size.y / 2.0f, size.y)) }
{
    m_sliderText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
    m_sliderText->SetSampleThreshold(1.5f);

    AddChildNode(m_sliderText);
    AddChildNode(m_sliderHandle);
}

void UiSlider::SetValue(const float value)
{
    m_sliderValue = value;
    m_sliderValue = min(m_sliderValue, m_maxValue);
    m_sliderValue = max(m_sliderValue, m_minValue);
    UpdateSliderPosition();
    UpdateText();
}

void UiSlider::SetMin(const float minValue)
{
    if (minValue > m_maxValue)
    {
        ULOGD("Slider " << GetName() << " in function ::SetMin failed! The min value is bigger than the current max");
        return;
    }

    m_minValue = minValue;
    UpdateSliderPosition();
}

void UiSlider::SetMax(const float maxValue)
{
    if (maxValue < m_minValue)
    {
        ULOGD("Slider " << GetName() << " in function ::SetMax failed! The max value is smaller than the current min");
        return;
    }

    m_maxValue = maxValue;
    UpdateSliderPosition();
}

void UiSlider::SetSize(const glm::vec2 size)
{
    UiBasicControl::SetSize(size);
    m_sliderText->SetSize(size);
    m_sliderHandle->SetSize(glm::vec2(size.y / 2.0f, size.y));

    UpdateSliderPosition();
    UpdateText();
}

void UiSlider::CreateResources()
{    m_sliderText->CreateResources();
    m_sliderHandle->CreateResources();
    UiBasicControl::CreateResources();
}

void UiSlider::HandleEvent(Event* event)
{
    UiBasicControl::HandleEvent(event);
}

void UiSlider::Update()
{
    if (!m_handleActive && m_sliderHandle->IsHover() && Application::GetMouse().IsButtonPressed(Mouse::Button::LEFT))
    {
        m_handleActive = true;
        m_handleGrabPosition = m_sliderHandle->GetGlobalPosition() - glm::vec2(Application::GetMouse().GetMousePosition());
    }
    else if (m_handleActive && !Application::GetMouse().IsButtonPressed(Mouse::Button::LEFT))
    {
        m_handleActive = false;
    }

    if (m_handleActive)
    {
        const auto mousePosition = Application::GetMouse().GetMousePosition();
        const auto sliderGlobalPosition = GetGlobalPosition();
        const auto sliderSize = GetSize();
        const auto handleSize = m_sliderHandle->GetSize();

        const auto offsetFromSlider = glm::vec2(mousePosition) - sliderGlobalPosition;
        m_sliderHandle->SetPosition(glm::vec2(offsetFromSlider.x + m_handleGrabPosition.x, 0.0f));

        if (m_sliderHandle->GetPosition().x < 0.0f)
        {
            m_sliderHandle->SetPosition(glm::vec2(0.0f, 0.0f));
        }
        else if (m_sliderHandle->GetPosition().x + handleSize.x > sliderSize.x)
        {
            m_sliderHandle->SetPosition(glm::vec2(sliderSize.x - handleSize.x, 0.0f));
        }

        UpdateSliderValue();
        UpdateText();
    }

    UiBasicControl::Update();
}

void UiSlider::Render()
{
    if (!IsVisible())
        return;

    Renderer::GetInstance().SetBlending(true);
    UiBasicControl::Render();
    Renderer::GetInstance().SetBlending(false);
}

void UiSlider::UpdateSliderValue()
{ 
    const auto usableRange = GetSize().x - m_sliderHandle->GetSize().x;
    m_sliderValue = ((m_sliderHandle->GetPosition().x / usableRange) * m_maxValue) - m_minValue;
}

void UiSlider::UpdateSliderPosition()
{
    const auto normalizedValue = (m_sliderValue + m_minValue) / m_maxValue;
    m_sliderHandle->SetPosition(glm::vec2((GetSize().x - m_sliderHandle->GetSize().x) * normalizedValue, 0.0f));
}

void UiSlider::UpdateText()
{
    std::wstringstream ws;
    ws << std::fixed << std::setprecision(2) << m_sliderValue;
    m_sliderText->SetString(ws.str());
}