#include "Ullpch.h"
#include "UiView3D.h"
#include "Rendering/Shapes/Cube.h"
#include "Scene/SceneObjects/Camera.h"
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
    auto camera = new Camera("Test Cube", &m_scene, UiArea::GetSize());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
    camera->CalculateProjectionMatrix();
    camera->CalculateViewMatrix();
    m_scene.SetMainCamera(camera);

    auto root = m_scene.GetRootNode();
    root->AddNode(camera);
    root->AddNode(new Cube("Main camera", &m_scene));
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
}

void UiView3D::Update()
{
    m_scene.Update();
    m_areaUpdated = true;
}

void UiView3D::Render()
{
    if(m_areaUpdated)
    {
        m_frameBuffer->Bind();

		RenderBackground();
        m_scene.Render();

		m_frameBuffer->Unbind();

		m_areaUpdated = false;
    }
}