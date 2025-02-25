#include "Ullpch.h"
#include "MainLayer.h"
#include "Event/Event.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Renderer.h"
#include "UiElement/UiRenderArea.h"
#include "Application/Application.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

MainLayer::MainLayer(const glm::uvec2 size) : Layer("mainLayer", glm::uvec2(0, 0), size, false)
{
    CreateLayout();
}

void MainLayer::SetWindow(const NotOwner<UllWindow>& window)
{
    m_titleBar->SetWindow(window);
    m_3DView->SetWindow(window);
}

void MainLayer::RenderLayerComponents()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    UiRenderArea::Render();

    ImGui::EndFrame();
    ImGui::Render();
}

void MainLayer::CreateLayout()
{
    //Assuming we are working with a 1280 x 720 pixel window
    //Then we can scale content up and down

    const auto initSize = GetSize();
    const auto initScale = GetScale();

    m_titleBar = std::make_shared<UiTitleBar>("titleBarElement", glm::vec2(0.f, 0.f), glm::vec2(initSize.x, 30.f * initScale));
    AddChildNode(m_titleBar);

    m_menuView = std::make_shared<UiMenuView>("menuElement", glm::vec2(0.f, (30.f * initScale) + 1), glm::vec2(260.f, initSize.y - (30.f * initScale) - 1.f));
    AddChildNode(m_menuView);

    m_3DView = std::make_shared<UiView3D>("view3DElement", glm::vec2((260.f + initScale) + 1.f, (30.f * initScale) + 1.f), glm::vec2(initSize.x - (260.f * initScale) - 1.f, initSize.y - (30.f * initScale) - 1.f));
    AddChildNode(m_3DView);
 }

void MainLayer::ResizeLayout()
{
    const auto size = GetSize();
    const auto scale = GetScale();
    
    m_titleBar->SetSize(glm::vec2(size.x, 30.f * scale));
    m_titleBar->ResizeControls();

    m_menuView->SetPosition(glm::vec2(0.f, (30.f * scale) + 1.f));
    m_menuView->SetSize(glm::vec2(260.f * scale, size.y - (30.f * scale) - 1.f));
    m_menuView->ResizeControls();

    m_3DView->SetPosition(glm::vec2((260.f * scale) + 1.0f, (30.f * scale) + 1.f));
    m_3DView->SetSize(glm::vec2(size.x - (260.f * scale) - 1.0f, size.y - (30.f * scale) - 1.f));
}