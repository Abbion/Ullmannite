#include "Ullpch.h"
#include "UiNumberField.h"
#include "Application/Application.h"

using namespace Ull;

namespace 
{
	constexpr double BLINK_INTERVAL = 0.6f;

	constexpr int zeroKeyInt = static_cast<int>(Keyboard::Key::ZERO);
	constexpr int nineKeyInt = static_cast<int>(Keyboard::Key::NINE);

	std::optional<wchar_t> parseNumber(const Keyboard::Key key, const bool parsePeriod)
	{
		const int keyInt = static_cast<int>(key);
		
		if (keyInt < zeroKeyInt || keyInt > nineKeyInt)
		{
			if (key == Keyboard::Key::PERIOD && parsePeriod) //Add a second argument to that function that allows period
				return  L'.';
			return std::nullopt;
		}

		const int value = keyInt - zeroKeyInt;
		return static_cast<wchar_t>(value + static_cast<int>(L'0'));
	}
}

UiNumberField::UiNumberField(const std::string& name, const glm::vec2 position, const glm::vec2 size, const float startNumber, const bool useDecimals) :
	UiBasicControl(name, position, size, UiControlType::UiNumberField),
	m_useDecimals{ useDecimals },
	m_numberText{ std::make_shared<UiText>(std::string(name + "Text"), glm::vec2(0.0f, 0.0f), size, L"")},
	m_cursor{ std::make_shared<UiSpace>(std::string(name + "Cursor"), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f))},
	m_selectedHighlight{ std::make_shared<UiSpace>(std::string(name + "SelectionHighlight"), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)) }
{
	m_selectedHighlight->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
	m_selectedHighlight->SetBackgroundColor(glm::vec4(0.0f, 1.0f, 1.0f, 0.6f));
	m_selectedHighlight->CreateResources();
	m_selectedHighlight->SetVisibility(false);
	AddChildNode(m_selectedHighlight);

	SetNumber(startNumber);
	m_numberText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
	m_numberText->SetSampleThreshold(1.5f);
	m_numberText->CreateResources();
	AddChildNode(m_numberText);

	m_cursor->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
	m_cursor->SetBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_cursor->SetVisibility(false);
	m_cursor->CreateResources();
	AddChildNode(m_cursor);
}

float UiNumberField::GetNumber() const
{
	if (m_useDecimals)
		return std::round(m_currentNumber * 100.0f) / 100.0f;
	return std::round(m_currentNumber);
}

void UiNumberField::SetNumber(float number)
{
	m_currentNumber = std::clamp(number, m_minNumber, m_maxNumber);
	m_numberText->SetString(std::format(L"{}", GetNumber()));
}

void UiNumberField::SetMinNumber(const float minNumber)
{
	m_minNumber = minNumber;
	SetNumber(m_currentNumber);
}

void UiNumberField::SetMaNumber(const float maxNumber)
{
	m_maxNumber = maxNumber;
	SetNumber(m_currentNumber);
}

void UiNumberField::HandleEvent(Event* event)
{
    if (event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::MouseDown:
		if (IsHover())
		{
			UpdateCursorPosition();

			if (m_isEdited == false)
			{
				m_fullSelect = CreateHightlightBox();
				m_cursor->SetVisibility(false);
			}
			else
			{
				m_fullSelect = false;
				m_cursor->SetVisibility(true);
				const auto mousePosition = glm::vec2(Application::GetMouse().GetMousePosition()) - GetGlobalPosition();
				m_cursorPosition = m_numberText->GetClosestLetterIndexToPoint(mousePosition);
				UpdateCursorPosition();
			}

			m_isEdited = true;
		}
		else
		{
			ConfirmValue();
		}
        break;

    case EventType::MouseUp:
        break;

	case EventType::KeyDown:
	{
		if (!m_isEdited)
			break;

		m_fullSelect = false;

		const auto key = static_cast<KeyDownEvent*>(event)->GetVal();

		if (key == Keyboard::Key::LEFT && m_cursorPosition > 0)
		{
			if (m_isEmpty)
				break;

			if (Application::GetKeyboard().IsKeyPressed(Keyboard::Key::L_CONTROL))
				m_cursorPosition = 0;
			else
				m_cursorPosition--;

			UpdateCursorPosition();
		}
		else if (key == Keyboard::Key::RIGHT && m_cursorPosition < m_numberText->GetString().length())
		{
			if (m_isEmpty)
				break;

			if (Application::GetKeyboard().IsKeyPressed(Keyboard::Key::L_CONTROL))
				m_cursorPosition = m_numberText->GetString().length();
			else
				m_cursorPosition++;

			
			UpdateCursorPosition();
		}
		else if (key == Keyboard::Key::BACKSPACE)
		{
			if (m_fullSelect)
			{
				ClearFiled();
			}

			if (m_cursorPosition > 0)
			{
				auto numberString = m_numberText->GetString();
				numberString.erase(m_cursorPosition - 1, 1);

				if (numberString.empty())
					m_isEmpty = true;

				m_numberText->SetString(numberString);
				m_numberText->Update();
				m_cursorPosition--;
				UpdateCursorPosition();
			}
		}
		else if (key == Keyboard::Key::HOME)
		{
			m_cursorPosition = 0;
			UpdateCursorPosition();
		}
		else if (key == Keyboard::Key::END)
		{
			const auto numberString = m_numberText->GetString();
			m_cursorPosition = numberString.length();
			UpdateCursorPosition();
		}
		else if (key == Keyboard::Key::ENTER)
		{
			ConfirmValue();
		}
		else 
		{
			const auto parsedNumber = parseNumber(key, m_useDecimals);
			if (!parsedNumber)
				break;

			if (m_fullSelect)
				ClearFiled();

			m_isEmpty = false;
			auto numberString = m_numberText->GetString();
			numberString.insert(numberString.begin() + m_cursorPosition, parsedNumber.value());
			m_numberText->SetString(numberString);
			m_numberText->Update();
			m_cursorPosition++;
			UpdateCursorPosition();
		}

		m_fullSelect = false;

		break;
	}

	case EventType::KeyUp:
		break;

    default:
        break;
    }

    UiBasicControl::HandleEvent(event);
}

void UiNumberField::Update()
{
	if (IsHover())
		Application::GetMouse().SetCursorMode(Mouse::Mode::IBEAM);
	else
		Application::GetMouse().SetCursorMode(Mouse::Mode::POINTER);

	m_selectedHighlight->SetVisibility(m_fullSelect);

	if (m_isEdited && m_fullSelect == false)
	{

		const auto cursorSize = m_numberText->GetFontSize();
		m_cursor->SetSize(glm::vec2(1.0f, cursorSize));
		m_cursor->CreateResources();

		const auto time = glfwGetTime();
		if (time > m_lsatBlinkTime + BLINK_INTERVAL)
		{
			m_lsatBlinkTime = time;
			m_cursor->SetVisibility(!m_cursor->IsVisible());
		}
		
	}

	UiBasicControl::Update();
}

void UiNumberField::Render()
{
	if (!IsVisible())
		return;

	Application::GetRenderer().SetBlending(Renderer::State::ENABLE);
	UiBasicControl::Render();
	Application::GetRenderer().SetBlending(Renderer::State::DISABLE);
}

void UiNumberField::UpdateCursorPosition()
{
	if (m_isEmpty)
	{
		const auto currentPosition = m_cursor->GetPosition();
		m_cursor->SetPosition(glm::vec2(GetSize().x / 2.0f, currentPosition.y));
		return;
	}

	const auto letterPosition = m_numberText->GetLetterPositionAtIndex(m_cursorPosition);
	const auto cursorSize = m_numberText->GetFontSize();
	m_cursor->SetPosition(letterPosition - glm::vec2(0.0f, cursorSize * 0.85f));

	ResetCursorBlink();
}

void UiNumberField::ResetCursorBlink()
{
	m_lsatBlinkTime = glfwGetTime();
	m_cursor->SetVisibility(true);
}

bool UiNumberField::CreateHightlightBox()
{
	const auto numberString = m_numberText->GetString();

	if (numberString.empty())
	{
		m_selectedHighlight->SetVisibility(false);
		return false;
	}

	const auto firstletterPosition = m_numberText->GetLetterPositionAtIndex(0);
	const auto lastLetterPosition = m_numberText->GetLetterPositionAtIndex(numberString.length());
	const auto cursorSize = m_numberText->GetFontSize();

	m_selectedHighlight->SetPosition(firstletterPosition - glm::vec2(0.0f, cursorSize * 0.85f));
	m_selectedHighlight->SetSize(glm::vec2(lastLetterPosition.x - firstletterPosition.x, cursorSize));
	m_selectedHighlight->SetVisibility(true);
	return true;
}

void UiNumberField::ClearFiled()
{
	auto numberString = m_numberText->GetString();
	numberString.clear();
	m_numberText->SetString(numberString);
	m_numberText->Update();
	m_isEmpty = true;
	m_cursorPosition = 0;
	UpdateCursorPosition();
}

void UiNumberField::ConfirmValue()
{
	m_isEdited = false;
	m_fullSelect = false;
	m_cursor->SetVisibility(false);
	m_cursorPosition = 0;

	try
	{
		const auto number = std::stof(m_numberText->GetString());
		SetNumber(number);
	}
	catch (...)
	{
		SetNumber(m_maxNumber);
	}
}