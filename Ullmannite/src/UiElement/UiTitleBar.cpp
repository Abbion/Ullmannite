#include "Ullpch.h"
#include "UiTitleBar.h"
#include "Resources/Fonts/IconCodes.h"

using namespace Ull;

namespace {
	constexpr auto ButtonWidthRatio = 1.5f;
}

UiTitleBar::UiTitleBar(const std::string& name, const glm::uvec2 position, const glm::uvec2 size) :
	UiElement(name, position, size, UiElementType::TitleBar)
{
	CreateControls();
}

void UiTitleBar::HandleEvent(Event* event)
{
	if (event->IsHandeled())
		return;

	switch (event->GetType())
	{	
	case EventType::WindowRestored:
		if (m_restoreButton->IsEnabled())
			m_restoreButton->SetEnabled(false);
	break;

	case EventType::WindowMaximized:
		if (!m_restoreButton->IsEnabled())
			m_restoreButton->SetEnabled(true);
	break;

	}

	UiElement::HandleEvent(event);
}

void UiTitleBar::CreateControls()
{
	const auto size = GetSize();

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::MINIMIZE) && m_minimizeButton == nullptr)
	{
		m_minimizeButton = std::make_shared<UiButton>(GetName() + "MinimizeWindowButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y));

		m_minimizeButton->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		m_minimizeButton->SetHoverColor(glm::vec4(0.38f, 0.38f, 0.38f, 1.0f));
		m_minimizeButton->CreateResources();

		m_minimizeButton->SetOnClickFunction([this](UiButton& buttonElement) {
			m_minimizeFunction();
			});

		auto minimizeButtonText = m_minimizeButton->GetTextControl();
		minimizeButtonText->SetFontSize(15);
		minimizeButtonText->SetSampleThreshold(-1);
		minimizeButtonText->SetPosition(glm::vec2(0, 3));
		minimizeButtonText->SetFont(FontTag::UI_ICON);
		minimizeButtonText->SetString(std::wstring{ static_cast<wchar_t>(Icon::MINIMIZE_WINDOW) });

		AddChildNode(m_minimizeButton);
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::RESTORE) && m_restoreButton == nullptr)
	{
		m_restoreButton = std::make_shared<UiToggle>(GetName() + "RestoreButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)),

		m_restoreButton->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		m_restoreButton->SetHoverColor(glm::vec4(0.38f, 0.38f, 0.38f, 1.0f));
		m_restoreButton->CreateResources();

		m_restoreButton->SetOnEnabledFunction([this](UiToggle& toggleElement) {
			auto restoreButtonText = toggleElement.GetTextControl();
			restoreButtonText->SetString(std::wstring{ static_cast<wchar_t>(Icon::RESTORE_WINDOW) });
			m_maximizeFunction();
			});
		m_restoreButton->SetOnDisambledFunction([this](UiToggle& toggleElement) {
			auto restoreButtonText = toggleElement.GetTextControl();
			restoreButtonText->SetString(std::wstring{ static_cast<wchar_t>(Icon::MAXIMIZE_WINDOW) });
			m_restoreFunction();
			});

		auto restoreButtonText = m_restoreButton->GetTextControl();
		restoreButtonText->SetFontSize(15);
		restoreButtonText->SetSampleThreshold(-1);
		restoreButtonText->SetFont(FontTag::UI_ICON);
		restoreButtonText->SetString(std::wstring{ static_cast<wchar_t>(Icon::MAXIMIZE_WINDOW) });

		AddChildNode(m_restoreButton);
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::CLOSE) && m_closeButton == nullptr)
	{
		m_closeButton = std::make_shared<UiButton>(GetName() + "CloseWindowButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y));

		m_closeButton->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		m_closeButton->SetHoverColor(glm::vec4(0.75f, 0.13f, 0.18f, 1.0f));
		m_closeButton->CreateResources();

		m_closeButton->SetOnClickFunction([this](UiButton& buttonElement) {
			m_closeFunction();
			});

		auto closeButtonText = m_closeButton->GetTextControl();
		closeButtonText->SetFontSize(15);
		closeButtonText->SetSampleThreshold(-1);
		closeButtonText->SetFont(FontTag::UI_ICON);
		closeButtonText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CLOSE) });

		AddChildNode(m_closeButton);
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::TITLE) && m_titleText == nullptr)
	{
		m_titleText = std::make_shared<UiText>(GetName() + "TitleText", glm::uvec2(0, 0), glm::uvec2(size.y, size.y), L"Title");

		m_titleText->SetSmoothingExceptance({ L'i', L'l' });
		m_titleText->SetFontSize(14);
		m_titleText->SetSampleThreshold(1.0f);
		AddChildNode(m_titleText);
	}
}

RectF UiTitleBar::GetGrabArea() const
{
	const auto globalPosition = GetGlobalPosition();
	const auto size = GetSize();

	const auto buttonCount = static_cast<float>((m_minimizeButton != nullptr) + (m_restoreButton != nullptr) + (m_closeButton != nullptr));
	const auto buttonWidth = ((float)size.y * ButtonWidthRatio);

	RectF grabArea;
	grabArea.x = globalPosition.x;
	grabArea.y = globalPosition.y;
	grabArea.width = size.x - (buttonWidth * buttonCount);
	grabArea.height = size.y;

	return grabArea;
}

void UiTitleBar::ResizeControls()
{
	const auto size = GetSize();
	const auto buttonWidth = ((float)size.y * ButtonWidthRatio);
	float buttonCount = 0.0;

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::CLOSE))
	{
		buttonCount += 1.0f;

		m_closeButton->SetPosition(glm::uvec2(size.x - buttonWidth, 0));
		m_closeButton->SetSize(glm::uvec2(buttonWidth, size.y));
		m_closeButton->Update();
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::RESTORE))
	{
		buttonCount += 1.0f;

		m_restoreButton->SetPosition(glm::uvec2(size.x - (buttonWidth * buttonCount), 0));
		m_restoreButton->SetSize(glm::uvec2(buttonWidth, size.y));
		m_restoreButton->Update();
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::MINIMIZE))
	{
		buttonCount += 1.0f;

		m_minimizeButton->SetPosition(glm::uvec2(size.x - (buttonWidth * buttonCount), 0));
		m_minimizeButton->SetSize(glm::uvec2(buttonWidth, size.y));
		m_minimizeButton->Update();
	}

	if (m_functionality & static_cast<Functionality>(TitleBarFunctionality::TITLE))
	{
		m_titleText->SetPosition(glm::uvec2(buttonCount * buttonWidth, 0.0f));
		m_titleText->SetSize(glm::uvec2(size.x - ((buttonCount * 2.0f) * buttonWidth), size.y));
		m_titleText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
		m_titleText->Update();
	}
}

void UiTitleBar::SetTitleText(const std::wstring& titleText)
{
	m_titleText->SetString(titleText);
}

void UiTitleBar::SetTitleBarFunctionality(const TitleBarFunctionality functionality, const State state)
{
	if (state == State::Enable)
	{
		m_functionality |= static_cast<Functionality>(functionality);
		CreateControls();
	}
	else
	{
		m_functionality &= (~static_cast<Functionality>(functionality));

		switch (functionality)
		{
		case TitleBarFunctionality::MINIMIZE:
			RemoveChildNode(m_minimizeButton);
			m_minimizeButton.reset();
			break;
		case TitleBarFunctionality::RESTORE:
			RemoveChildNode(m_restoreButton);
			m_restoreButton.reset();
			break;
		case TitleBarFunctionality::CLOSE:
			RemoveChildNode(m_closeButton);
			m_restoreButton.reset();
			break;
		case TitleBarFunctionality::TITLE:
			RemoveChildNode(m_titleText);
			m_titleText.reset();
			break;
		}
	}
}
