#include "Ullpch.h"
#include "UiTitleBar.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"

#include "Rendering/IconsCode/IconCodes.h"

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
	m_minimizeButton{ std::make_shared<UiButton>("minimizeWindowButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)) },
	m_restoreButton{ std::make_shared<UiToggle>("restoreButton", glm::uvec2(0, 0), glm::uvec2(size.y, size.y)) },
	m_titleText{ std::make_shared<UiText>("titleText", glm::uvec2(0, 0), glm::uvec2(size.y, size.y), L"Ullmanite")  }
{
	m_titleText->SetSmoothingExceptance({ L'i', L'l' });
	CreateControls();

	SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
}

void UiTitleBar::CreateResources()
{
	UiArea::CreateResources();	//TODO: What does it do

	const auto buttonWidth = ((float)m_size.y * 1.5f);
	m_window->SetDragArea(glm::uvec2(0u, 0u), glm::uvec2(m_size.x - (3.0f * buttonWidth), m_size.y));

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

	for (auto& child : m_children)
		child->HandleEvent(event);
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

	for (auto& child : m_children)
		child->Render();
}

void UiTitleBar::Render()
{
	if (m_areaUpdated)
	{
		m_frameBuffer->Bind();
		RenderBackground();

		for (auto& child : m_children)
			child->Render();

		m_frameBuffer->Unbind();

		m_areaUpdated = false;
	}

	RenderUI();
}

void UiTitleBar::RenderUI()
{
	/*
	ImGui::SetNextWindowSize(ImVec2((float)m_size.x, (float)m_size.y));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	//TODO: Move this out of here???
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding.y = 0.0f;
	style.WindowPadding.x = 0.0f;
	//-----------------------

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin("TitleBarImGui", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleVar();

	//Title group
	ImGui::BeginGroup();

	auto windowSize = ImGui::GetWindowSize();
	auto textSize = ImGui::CalcTextSize("Ullmanite");

	ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
	ImGui::SetCursorPosY((windowSize.y - textSize.y) * 0.5f);

	ImGui::Text("Ullmanite");

	ImGui::EndGroup();
	//-----------------------

	//Button Group
	ImGui::BeginGroup();

	auto buttonHeight = m_size.y;
	float nextPos = m_size.x - (3 * buttonWidth);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));

	ImGui::SetWindowFontScale(0.6f);

	ImGui::SameLine(nextPos);
	if (ImGui::Button(ICON_MINIMIZE, ImVec2(buttonWidth, (float)buttonHeight)))
		m_minimizePressed = true;

	nextPos += buttonWidth;

	ImGui::SameLine(nextPos);
	auto maximizeRestoreIcon = m_window->IsMaximized() ? ICON_RESTORE : ICON_MAXIMIZE;

	if (ImGui::Button(maximizeRestoreIcon, ImVec2(buttonWidth, (float)buttonHeight)))
		m_maximizeRestorPressed = true;

	nextPos += buttonWidth;

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::SameLine(nextPos);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	
	if (ImGui::Button(ICON_CLOSE, ImVec2(buttonWidth, (float)buttonHeight)))
		m_closePressed = true;

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::SetWindowFontScale(1.0);

	ImGui::EndGroup();
	//-----------------------

	ImGui::End();
	*/
}

void UiTitleBar::CreateControls()
{
	m_closeButton->SetParent(this);
	m_closeButton->SetHoverColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_closeButton->CreateResources();

	m_closeButton->SetOnClickFunction([this](UiButton& buttonElement) {
		m_window->Close();
	});

	AddUiElement(m_closeButton);

	m_minimizeButton->SetParent(this);
	m_minimizeButton->SetHoverColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_minimizeButton->CreateResources();

	m_minimizeButton->SetOnClickFunction([this](UiButton& buttonElement) {
		m_window->Minimize();
	});

	AddUiElement(m_minimizeButton);

	m_restoreButton->SetParent(this);
	m_restoreButton->SetHoverColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	m_restoreButton->CreateResources();

	m_restoreButton->SetOnEnabledFunction([this](UiToggle& buttonElement) {
		m_window->Maximize();
	});
	m_restoreButton->SetOnDisambledFunction([this](UiToggle& buttonElement) {
		m_window->Restore();
	});

	AddUiElement(m_restoreButton);

	m_titleText->SetFontSize(14);
	m_titleText->SetSampleThreshold(1.0f);
	m_titleText->SetParent(this);
	AddUiElement(m_titleText);
}

void UiTitleBar::ResizeControls()
{
	const auto buttonWidth = (unsigned)((float)m_size.y * 1.5f);

	m_closeButton->SetPositiion(glm::uvec2(m_size.x - buttonWidth, 0));
	m_closeButton->SetSize(glm::uvec2(buttonWidth, m_size.y));
	m_closeButton->Update();

	m_restoreButton->SetPositiion(glm::uvec2(m_size.x - (buttonWidth * 2.f), 0));
	m_restoreButton->SetSize(glm::uvec2(buttonWidth, m_size.y));
	m_restoreButton->Update();

	m_minimizeButton->SetPositiion(glm::uvec2(m_size.x - (buttonWidth * 3.f), 0));
	m_minimizeButton->SetSize(glm::uvec2(buttonWidth, m_size.y));
	m_minimizeButton->Update();

	m_titleText->SetPositiion(glm::uvec2(3.0f * buttonWidth, 0.0f));
	m_titleText->SetSize(glm::uvec2(m_size.x - (6.0f * buttonWidth), m_size.y));
	m_titleText->SetAlignment(UiText::HorizontalAlignment::CENTER, UiText::VerticalAlignment::CENTER);
	m_titleText->Update();
}