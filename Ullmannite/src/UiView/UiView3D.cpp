#include "Ullpch.h"
#include "UiView3D.h"
#include "Rendering/Objects/Cube.h"
#include "Rendering/Objects/MarchCubeRenderer.h"
#include "Rendering/Objects/DirectionalLight.h"
#include "Scene/SceneObjects/Camera.h"
#include "Rendering/Api/Renderer.h"
#include "Utilities/CollisionCheckers.h"
#include <string>

#include "DataLoaders/VolumeLoader.h"
#include "DataStructures/VolumeData.h"

#include <algorithm>

using namespace Ull;

UiView3D::UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiArea(name, position, size, true),
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

    //TEMP data loading
    auto mainDataSet = LoadVolumeData("Assets/VolumetricData/volumeTest.dat");

    auto cubeMarch = new MarchCubeRenderer("Cube march", &m_scene);
    cubeMarch->SetVolumeData(mainDataSet);
    cubeMarch->GenerateMesh();
    root->AddNode(cubeMarch);
    //auto cube = new Cube("Test Cube", &m_scene);
    //root->AddNode(cube);
}

void UiView3D:: HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::WindowResize:
    {
         auto cameraNode = m_scene.GetNodeByName("Main camera");
        
        if(cameraNode != nullptr)
        {
            auto camera = static_cast<Camera*>(cameraNode);
            camera->SetRenderAreaSize(UiArea::GetSize());
        }
    }
    break;
    
    case EventType::MouseMove:
    case EventType::MouseDown:
    case EventType::MouseUp:
    case EventType::MouseScroll:
    {
        if(!PointInStaticRect<glm::ivec2>(Mouse::GetInstance().GetMousePosition(), GetPosition(), GetSize()) && !m_window->IsCursorLocked())
            return;
    }
    break;

    default:
        break;
    }

    m_scene.HandleEvent(event);
}

void UiView3D::Update()
{
    m_scene.Update();

    if(m_scene.IsUpdated())
    {
        m_areaUpdated = true;
        m_scene.SetUpdated(false);
    }
}

void UiView3D::SetWindow(const NotOwner<UllWindow>& window)
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

        Renderer::GetInstance().SetDepth(Renderer::State::ENABLE);
        Renderer::GetInstance().Clear(Renderer::ClearBits::DEPTH);

        //Renderer::GetInstance().SetFaceCulling(Renderer::FaceCulling::BACK);
        //Renderer::GetInstance().SetFaceWinding(Renderer::FaceWinding::COUNTER_CLOCKWISE);
        m_scene.Render();
        //Renderer::GetInstance().SetFaceCulling(Renderer::FaceCulling::NONE);
        //Renderer::GetInstance().SetDepth(Renderer::State::DISABLE);

		m_frameBuffer->Unbind();

		m_areaUpdated = false;
    }
}