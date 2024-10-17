#include "Ullpch.h"
#include "UiTitleBar.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"
#include "Resources/Fonts/IconCodes.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

namespace
{
	constexpr float buttonWidth = 45.f;
}

UiTitleBar::UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size) :
	UiArea(name, position, size, false),
	m_closeButton{ std::make_shared<UiButton>("closeWindowButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)) },
	m_restoreButton{ std::make_shared<UiToggle>("restoreButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)) },
	m_minimizeButton{ std::make_shared<UiButton>("minimizeWindowButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)) },
	m_titleText{ std::make_shared<UiText>("titleText", glm::uvec2(0, 0), glm::uvec2(size.y, size.y), L"Ullmanite")  }
{
	m_titleText->SetSmoothingExceptance({ L'i', L'l' });
	CreateControls();

	SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
}

void UiTitleBar::CreateResources()
{
	CreateFrameBuffer();

	const auto size = GetSize();
	auto areaSizeChangedEvent = std::make_shared<RenderAreaSizeChanged>(EventType::RenderAreaSizeChanged);
	UiArea::HandleEvent(areaSizeChangedEvent.get());

	const auto buttonWidth = ((float)size.y * 1.5f);
	m_window->SetDragArea(glm::uvec2(0u, 0u), glm::uvec2(size.x - (3.0f * buttonWidth), size.y));

	ResizeControls();
}

void UiTitleBar::HandleEvent(Event* event)
{
	if (event->IsHandeled())
		return;

	switch (event->GetType())
	{	
	case EventType::WindowRestored:
		m_areaUpdated = true;
		if (m_restoreButton->IsEnabled())
			m_restoreButton->SetEnabled(false);
	break;

	case EventType::WindowMaximized:
		if (!m_restoreButton->IsEnabled())
			m_restoreButton->SetEnabled(true);
	break;

	}

	UiArea::HandleEvent(event);
}

void UiTitleBar::Update()
{
	if (m_minimizePressed)
	{
		m_window->Minimize();
		m_minimizePressed = false;
	}
	else if (m_maximizeRestorPressed)
	{
		if (m_window->IsMaximized())
			m_window->Restore();
		else
			m_window->Maximize();
		
		m_maximizeRestorPressed = false;
	}
	else if (m_closePressed)
	{
		m_window->Close();
		m_closePressed = false;
	}

	UiArea::Update();
}

void UiTitleBar::Render()
{
	UiArea::Render();
}

void UiTitleBar::CreateControls()
{
	m_closeButton->SetBackgroundColor(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	m_closeButton->SetHoverColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
	m_closeButton->CreateResources();

	m_closeButton->SetOnClickFunction([this](UiButton& buttonElement) {
		m_window->Close();
	});

	auto& closeButtonText = m_closeButton->GetTextControl();
	closeButtonText.SetFontSize(14);
	closeButtonText.SetFont(FontTag::UI_ICON);
	closeButtonText.SetString(std::wstring{ static_cast<wchar_t>(Icon::CLOSE)});

	AddChildNode(m_closeButton);

	m_restoreButton->SetBackgroundColor(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	m_restoreButton->SetHoverColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_restoreButton->CreateResources();

	m_restoreButton->SetOnEnabledFunction([this](UiToggle& buttonElement) {
		m_window->Maximize();
		});
	m_restoreButton->SetOnDisambledFunction([this](UiToggle& buttonElement) {
		m_window->Restore();
		});

	auto& restoreButtonText = m_restoreButton->GetTextControl();
	restoreButtonText.SetFontSize(14);
	restoreButtonText.SetFont(FontTag::UI_ICON);
	restoreButtonText.SetString(std::wstring{ static_cast<wchar_t>(Icon::MAXIMIZE_WINDOW) });

	AddChildNode(m_restoreButton);

	m_minimizeButton->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));
	m_minimizeButton->SetHoverColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_minimizeButton->CreateResources();

	m_minimizeButton->SetOnClickFunction([this](UiButton& buttonElement) {
		m_window->Minimize();
	});

	auto& minimizeButtonText = m_minimizeButton->GetTextControl();
	minimizeButtonText.SetFontSize(14);
	minimizeButtonText.SetFont(FontTag::UI_ICON);
	minimizeButtonText.SetString(std::wstring{ static_cast<wchar_t>(Icon::MINIMIZE_WINDOW)} );

	AddChildNode(m_minimizeButton);

	m_titleText->SetFontSize(14);
	m_titleText->SetSampleThreshold(1.0f);
	AddChildNode(m_titleText);
}

void UiTitleBar::ResizeControls()
{
	const auto size = GetSize();

	const auto buttonWidth = (unsigned)((float)size.y * 1.5f);

	m_closeButton->SetPosition(glm::uvec2(size.x - buttonWidth, 0));
	m_closeButton->SetSize(glm::uvec2(buttonWidth, size.y));
	m_closeButton->Update();

	m_restoreButton->SetPosition(glm::uvec2(size.x - (buttonWidth * 2.f), 0));
	m_restoreButton->SetSize(glm::uvec2(buttonWidth, size.y));
	m_restoreButton->Update();

	m_minimizeButton->SetPosition(glm::uvec2(size.x - (buttonWidth * 3.f), 0));
	m_minimizeButton->SetSize(glm::uvec2(buttonWidth, size.y));
	m_minimizeButton->Update();

	m_titleText->SetPosition(glm::uvec2(3.0f * buttonWidth, 0.0f));
	m_titleText->SetSize(glm::uvec2(size.x - (6.0f * buttonWidth), size.y));
	m_titleText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
	m_titleText->Update();
}