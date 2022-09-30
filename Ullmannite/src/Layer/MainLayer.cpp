#include "Ullpch.h"
#include "MainLayer.h"
#include "Event/Event.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Rendering/Api/ShaderManager.h"
#include "Rendering/Api/Renderer.h"
#include "UiElement/UiArea.h"
#include "UiElement/UiTitleBar.h"

using namespace Ull;

MainLayer::MainLayer(glm::uvec2 size) : Layer("main", size)
{
    Init();
}

void MainLayer::Update()
{
}

void MainLayer::Render()
{
    for (auto& element : m_layout->GetChildren())
    {
        element->Render();
    }

    m_layout->Render();
}

void MainLayer::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::WindowResize:
        Resize(static_cast<WindowResizeEvent*>(event)->GetVal());
    break;
    }

    m_layout->HandleEvent(event);
}

void MainLayer::Init()
{
    CreateLayout();
}

void MainLayer::CreateLayout()
{
    //Assuming we are woring with a 1280 x 720 pixel window
    //Then we can scale content up and down

    auto initSize = m_layout->GetSize();

    m_viewMatrix = glm::ortho(0.0f, (float)initSize.x, (float)initSize.y, 0.0f, -1.0f, 1.0f);

    UiTitleBar* titleBarView = new UiTitleBar("titleBarElement", glm::vec2(0, 0), glm::vec2(initSize.x, 30));
    m_layout->AddUiElement(titleBarView);

    UiArea* menuView = new UiArea("menuElement", glm::vec2(0, 30), glm::vec2(260, initSize.y - 30));
    menuView->SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.15f, 1.0f));
    m_layout->AddUiElement(menuView);

    UiArea* renderView = new UiArea("renderElement", glm::vec2(260, 30), glm::vec2(initSize.x - 260, initSize.y - 30));
    renderView->SetBackgroundColor(glm::vec4(0.35f, 0.22f, 0.31f, 1.0f));
    m_layout->AddUiElement(renderView);

    m_layout->CreateResources();
}

void MainLayer::Resize(const glm::uvec2& size)
{
    m_viewMatrix = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);

    m_layout->SetSize(size);
    m_layout->CreateResources();
    
    for(auto& element : m_layout->GetChildren())
    {
        auto currentElementName = element->GetName(); 

        if(currentElementName == "titleBarElement")
        {
            element->SetSize(glm::vec2(size.x, 30));
        }

        else if(currentElementName == "menuElement")
        {
            element->SetSize(glm::vec2(260, size.y - 30));
        }

        else if(currentElementName == "renderElement")
        {
            element->SetPositiion(glm::vec2(260, 30));
            element->SetSize(glm::vec2(size.x - 260, size.y - 30));
        }

        element->CreateResources();
    }
}