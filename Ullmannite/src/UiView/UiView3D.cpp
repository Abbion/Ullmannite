#include "Ullpch.h"
#include "UiView3D.h"
#include "Rendering/Shapes/Cube.h"
#include "Scene/SceneObjects/Camera.h"
#include "Rendering/Api/Renderer.h"
#include <string>

using namespace Ull;

UiView3D::UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiArea(name, position, size),
    m_scene("Scene 3D")
{
    SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

    Init();
}

void UiView3D::Init()
{
    auto camera = new Camera("Main camera", &m_scene, UiArea::GetSize());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->CalculateProjectionMatrix();
    camera->CalculateViewMatrix();

    m_scene.SetMainCamera(camera);

    auto root = m_scene.GetRootNode();
    root->AddNode(camera);

    auto cube = new Cube("Test Cube", &m_scene);
    root->AddNode(cube);
}

void UiView3D:: HandleEvent(Event* event)
{
    if(event->GetType() == EventType::WindowResize)
    {
        auto cameraNode = m_scene.GetNodeByName("Main camera");
        
        if(cameraNode != nullptr)
        {
            auto camera = static_cast<Camera*>(cameraNode);
            camera->SetRenderAreaSize(UiArea::GetSize());
        }
    }

    m_scene.HandleEvent(event);
}

void UiView3D::Update()
{
    m_scene.Update();
    m_areaUpdated = true;
}

void UiView3D::SetWindow(const std::shared_ptr<UllWindow>& window)
{
    m_window = window;
    auto cameraNode = m_scene.GetNodeByName("Main camera");
    if(cameraNode != nullptr)
    {
        auto camera = static_cast<Camera*>(cameraNode);
        camera->SetWindow(m_window);
    }
}

void UiView3D::Render()
{
    if(m_areaUpdated)
    {
        m_frameBuffer->Bind();

		RenderBackground();

        //Renderer::GetInstance().SetDepth(Renderer::State::ENABLE);
        m_scene.Render();
        //Renderer::GetInstance().SetDepth(Renderer::State::DISABLE);

		m_frameBuffer->Unbind();

		//m_areaUpdated = false;
    }
}