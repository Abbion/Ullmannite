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

namespace
{
    constexpr auto TitleBarHeight = 30.0f;
    constexpr auto MenuWidth = 260.0f;
}

MainLayer::MainLayer(const glm::uvec2 size, const NotOwner<LayerManager>& layerManager) : Layer(LayerNames::mainLayer, glm::uvec2(0, 0), size, false, layerManager)
{
    CreateLayout();
}

void MainLayer::RenderLayerComponents()
{
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplGlfw_NewFrame();

    //ImGui::NewFrame();

    UiRenderArea::Render();

    //ImGui::EndFrame();
    //ImGui::Render();
}

void MainLayer::CreateLayout()
{
    //Assuming we are working with a 1280 x 720 pixel window
    //Then we can scale content up and down

    const auto initSize = GetSize();
    const auto initScale = GetScale();

    m_titleBarView = std::make_shared<UiRenderArea>(GetName() + "titleBarAreaView", glm::vec2(0.0f, 0.0f), glm::vec2(initSize.x, TitleBarHeight * initScale), false);
    m_titleBarView->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));

    m_titleBar = std::make_shared<UiTitleBar>(GetName() + "titleBar", glm::vec2(0.f, 0.f), glm::vec2(initSize.x, TitleBarHeight * initScale));
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::MINIMIZE, State::Enable);
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::RESTORE, State::Enable);
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::CLOSE, State::Enable);
    m_titleBar->SetTitleBarFunctionality(UiTitleBar::TitleBarFunctionality::TITLE, State::Enable);
    m_titleBar->SetTitleText(L"Ullmannite");

    m_titleBar->SetCloseFunction([this] { 
        auto& window = Application::GetWindow();
        window.Close();
    });
    m_titleBar->SetMinimizeFunction([this] { 
        auto& window = Application::GetWindow();
        window.Minimize();
    });
    m_titleBar->SetMaximizeFunction([this] {
        auto& window = Application::GetWindow();
        window.Maximize();
    });
    m_titleBar->SetRestoreFunction([this] { 
        auto& window = Application::GetWindow();
        window.Restore(); 
    });

    m_titleBarView->AddChildNode(m_titleBar);
    AddChildNode(m_titleBarView);

    m_menuView = std::make_shared<UiMenuView>("menuElement", glm::vec2(0.f, (TitleBarHeight * initScale) + 1), glm::vec2(MenuWidth, initSize.y - (TitleBarHeight * initScale) - 1.f));
    AddChildNode(m_menuView);

    m_3DView = std::make_shared<UiView3D>("view3DElement", glm::vec2((MenuWidth + initScale) + 1.f, (TitleBarHeight * initScale) + 1.f), glm::vec2(initSize.x - (MenuWidth * initScale) - 1.f, initSize.y - (TitleBarHeight * initScale) - 1.f));
    AddChildNode(m_3DView);
 }

void MainLayer::ResizeLayout()
{
    const auto size = GetSize();
    const auto scale = GetScale();
    auto& window = Application::GetWindow();
    
    m_titleBarView->SetSize(glm::vec2(size.x, TitleBarHeight * scale));
    m_titleBar->SetSize(m_titleBarView->GetSize());
    m_titleBar->ResizeControls();

    const auto grabArea = m_titleBar->GetGrabArea();
    window.SetDragArea(glm::uvec2(0u, 0u), glm::uvec2(grabArea.width, grabArea.height));
    for (auto layer : m_layerManager->GetLayers())
    {
        if (layer->GetName() == LayerNames::toolLayer)
        {
            layer->SetSize(glm::vec2(size.x, size.y - grabArea.height));
            layer->SetPosition(glm::vec2(0.0f, grabArea.height));
            break;
        }
    }

    m_menuView->SetPosition(glm::vec2(0.f, (TitleBarHeight * scale) + 1.f));
    m_menuView->SetSize(glm::vec2(MenuWidth * scale, size.y - (TitleBarHeight * scale) - 1.f));
    m_menuView->ResizeControls();

    m_3DView->SetPosition(glm::vec2((MenuWidth * scale) + 1.0f, (TitleBarHeight * scale) + 1.f));
    m_3DView->SetSize(glm::vec2(size.x - (MenuWidth * scale) - 1.0f, size.y - (TitleBarHeight * scale) - 1.f));
}