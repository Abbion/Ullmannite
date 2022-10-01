#include "Ullpch.h"
#include "UiTitleBar.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

namespace
{
	constexpr float buttonWidth = 45.f;

	inline bool ResetAndReturnPressState(bool& pressState)
	{
		bool currentPressState = pressState;
		pressState = false;
		return currentPressState;
	}
}

UiTitleBar::UiTitleBar(std::string name, glm::uvec2 position, glm::uvec2 size) :
	UiArea(name, position, size)
{
	SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
}

bool UiTitleBar::WasClosePressed()
{
	return ResetAndReturnPressState(m_closePressed);
}

bool UiTitleBar::WasMaximizeRestorePressed()
{
	return ResetAndReturnPressState(m_maximizeRestorPressed);
}

bool UiTitleBar::WasMinimizePressed()
{
	return ResetAndReturnPressState(m_minimizePressed);
}

void UiTitleBar::CreateResources()
{
	UiArea::CreateResources();
}

void UiTitleBar::HandleEvent(Event* event)
{
	if (event->GetType() == EventType::MouseMove)
	{
		if (PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize()))
			m_areaUpdated = true;
	}
	if (event->GetType() == EventType::MouseDown)
	{
		m_areaUpdated = true;
	}
	if (event->GetType() == EventType::MouseUp)
	{
		m_areaUpdated = true;
	}
}

void UiTitleBar::Update()
{
	
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
	if (ImGui::Button("_", ImVec2(buttonWidth, buttonHeight)))
		m_minimizePressed = true;

	nextPos += buttonWidth;

	ImGui::SameLine(nextPos);
	if (ImGui::Button("O", ImVec2(buttonWidth, buttonHeight)))
		m_maximizeRestorPressed = true;

	nextPos += buttonWidth;

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::SameLine(nextPos);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.909f, 0.282f, 0.345f, 1.0f));
	
	if (ImGui::Button("X", ImVec2(buttonWidth, buttonHeight)))
	{
		ULOGD("X");
		m_closePressed = true;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::EndGroup();
	//-----------------------

	ImGui::End();
}