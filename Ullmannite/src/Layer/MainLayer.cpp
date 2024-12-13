#include "Ullpch.h"
#include "MainLayer.h"
#include "Event/Event.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Renderer.h"
#include "UiElement/UiRenderArea.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

MainLayer::MainLayer(glm::uvec2 size) : Layer("main", size)
{
    CreateLayout();
}

void MainLayer::SetWindow(const NotOwner<UllWindow>& window)
{
    m_titleBar->SetWindow(window);
    m_3DView->SetWindow(window);
}

void MainLayer::HandleEvent(Event* event)
{
    if(event->IsHandeled())
        return;

    switch (event->GetType())
    {
    case EventType::WindowResize:
        m_layout->SetSize(static_cast<WindowResizeEvent*>(event)->GetVal());
        Resize(m_layout->GetSize());
    break;

    case EventType::UiScaledUp:
        m_layout->SetScale(m_layout->GetScale() * static_cast<UiScaledUpEvent*>(event)->GetVal());
        Resize(m_layout->GetSize());
    break;

    case EventType::UiScaledDown:
        m_layout->SetScale(m_layout->GetScale() * static_cast<UiScaledDownEvent*>(event)->GetVal());
        Resize(m_layout->GetSize());
    break;
    }

    m_layout->HandleEvent(event);
}

void MainLayer::Update()
{
    for (auto& element : m_layout->GetChildren())
    {
        element->Update();
    }
}

void MainLayer::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    for (auto& element : m_layout->GetChildren())
    {
        element->Render();
    }

    ImGui::EndFrame();
    ImGui::Render();

    m_layout->Render();
}

void MainLayer::CreateLayout()
{
    //Assuming we are working with a 1280 x 720 pixel window
    //Then we can scale content up and down

    const auto initSize = m_layout->GetSize();
    const auto initScale = m_layout->GetScale();
    m_layout->CreateResources();

    m_titleBar = std::make_shared<UiTitleBar>("titleBarElement", glm::vec2(0.f, 0.f), glm::vec2(initSize.x, 30.f * initScale));
    m_layout->AddChildNode(m_titleBar);

    m_menuView = std::make_shared<UiMenuView>("menuElement", glm::vec2(0.f, (30.f * initScale) + 1), glm::vec2(260.f, initSize.y - (30.f * initScale) - 1.f));
    m_layout->AddChildNode(m_menuView);

    m_3DView = std::make_shared<UiView3D>("view3DElement", glm::vec2((260.f + initScale) + 1.f, (30.f * initScale) + 1.f), glm::vec2(initSize.x - (260.f * initScale) - 1.f, initSize.y - (30.f * initScale) - 1.f));
    m_3DView->SetTransferFunction(m_menuView->GetTransferFunctionRenderer());
    m_layout->AddChildNode(m_3DView);
}

void MainLayer::Resize(const glm::uvec2& size)
{
    m_layout->CreateResources();
    const auto scale = m_layout->GetScale();
    
    m_titleBar->SetSize(glm::vec2(size.x, 30.f * scale));
    m_titleBar->CreateResources();

    m_menuView->SetPosition(glm::vec2(0.f, (30.f * scale) + 1.f));
    m_menuView->SetSize(glm::vec2(260.f * scale, size.y - (30.f * scale) - 1.f));
    m_menuView->CreateResources();

    m_3DView->SetPosition(glm::vec2((260.f * scale) + 1.0f, (30.f * scale) + 1.f));
    m_3DView->SetSize(glm::vec2(size.x - (260.f * scale) - 1.0f, size.y - (30.f * scale) - 1.f));
    m_3DView->CreateResources();
}