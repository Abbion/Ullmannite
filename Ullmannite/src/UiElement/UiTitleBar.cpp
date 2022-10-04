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
	UiArea(name, position, size)
{
	SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
}

void UiTitleBar::CreateResources()
{
	UiArea::CreateResources();
}

void UiTitleBar::HandleEvent(Event* event)
{
	switch (event->GetType())
	{
	case EventType::MouseMove:
		if (PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize()))
			m_areaUpdated = true;
		break;
	
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderUI();

		ImGui::Render();
	
		m_frameBuffer->Unbind();

		m_areaUpdated = false;
	}
}

void UiTitleBar::RenderUI()
{
	ImGui::SetNextWindowSize(ImVec2(m_size.x, m_size.y));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

	//Move this out of here???
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

	ImGui::SameLine(nextPos);
	if (ImGui::Button(ICON_MINIMIZE, ImVec2(buttonWidth, buttonHeight)))
		m_minimizePressed = true;

	nextPos += buttonWidth;

	ImGui::SameLine(nextPos);
	auto maximizeRestoreIcon = m_window->IsMaximized() ? ICON_RESTORE : ICON_MAXIMIZE;

	if (ImGui::Button(maximizeRestoreIcon, ImVec2(buttonWidth, buttonHeight)))
		m_maximizeRestorPressed = true;

	nextPos += buttonWidth;

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::SameLine(nextPos);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	
	if (ImGui::Button(ICON_CLOSE, ImVec2(buttonWidth, buttonHeight)))
		m_closePressed = true;

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndGroup();
	//-----------------------

	ImGui::End();
}