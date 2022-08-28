#include "Ullpch.h"
#include "MainLayer.h"
#include "Event/Event.h"

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
    m_layout->Render();
    for (auto& element : m_layout->GetChildren())
    {
        element->Render();
    }
}

void MainLayer::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::WindowResize:
        Resize(static_cast<WindowResizeEvent*>(event)->GetVal());
    break;
    }
}

void MainLayer::Init()
{
    CreateLayout();
    m_layout->SetColor(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
}

void MainLayer::CreateLayout()
{
    //Assuming we are woring with a 1280 x 720 pixel window
    //Then we can scale content up and down

    auto initSize = m_layout->GetSize();

    UiArea* topBarView = new UiArea("topBarElement", glm::vec2(0, 0), glm::vec2(initSize.x, 30));
    topBarView->SetColor(glm::vec4(0.0f, 0.56f, 0.55f, 1.0f));
    m_layout->AddUiElement(topBarView);

    UiArea* renderView = new UiArea("renderElement", glm::vec2(0, 30), glm::vec2(initSize.x - 260, initSize.y));
    renderView->SetColor(glm::vec4(0.0f, 0.35f, 0.35f, 1.0f));
    m_layout->AddUiElement(renderView);

    UiArea* menuView = new UiArea("menuElement", glm::vec2(initSize.x - 260, 30), glm::vec2(260, initSize.y - 30));
    menuView->SetColor(glm::vec4(0.0f, 0.2f, 0.2f, 1.0f));
    m_layout->AddUiElement(menuView);

    m_layout->CreateResources();
}

void MainLayer::Resize(const glm::uvec2& size)
{
    ULOGD(size.x << ", " << size.y);
    m_layout->SetSize(size);
    m_layout->CreateResources();
    
    for(auto& element : m_layout->GetChildren())
    {
        auto currentElementName = element->GetName(); 
        if(currentElementName == "topBarElement")
        {
            element->SetSize(glm::vec2(size.x, 30));
        }
        
        else if(currentElementName == "renderElement")
        {
            element->SetSize(glm::vec2(size.x - 260, size.y - 30));
        }
        else if(currentElementName == "menuElement")
        {
            element->SetPositiion(glm::vec2(size.x - 260, 30));
            element->SetSize(glm::vec2(260, size.y - 30));
        }

        element->CreateResources();
    }
}