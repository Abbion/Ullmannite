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
	UiArea(name, position, size, false)
{
	SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
}

void UiTitleBar::CreateResources()
{
	UiArea::CreateResources();	//TODO: What does it do
}

void UiTitleBar::HandleEvent(Event* event)
{
	if (event->IsHandeled())
		return;

	switch (event->GetType())
	{	
	case EventType::MouseDown:
		if (PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize() - glm::uvec2(3 * buttonWidth, 0)))
			m_window->EnableDrag(true);
		else
			m_window->EnableDrag(false);

		m_areaUpdated = true;
		break;

	case EventType::MouseUp:
		m_window->EnableDrag(false);
		m_areaUpdated = true;
		break;

	case EventType::WindowRestored:
		m_areaUpdated = true;
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
}

void UiTitleBar::Render()
{
	if (m_areaUpdated)
	{
		m_frameBuffer->Bind();
		RenderBackground();
		m_frameBuffer->Unbind();

		m_areaUpdated = false;
	}

	RenderUI();
}

void UiTitleBar::RenderUI()
{
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
}