#include "Ullpch.h"
#include "UiView3D.h"
#include "Application/Application.h"
#include "UiMenuView.h"
#include "Rendering/Objects/Cube.h"
#include "Rendering/Objects/MarchCubeRenderer.h"
#include "Rendering/Objects/DirectionalLight.h"
#include "Rendering/Utils/RenderHelper.h"
#include "Scene/SceneObjects/Camera.h"
#include "Utilities/CollisionCheckers.h"
#include <string>
#include <algorithm>
#include "Core/PlatformDependantFreeFunctions.h"
#include <codecvt>
#include <locale>

using namespace Ull;

namespace 
{
    std::string ConvertDcmToDat(std::string str)
    {
        auto pos = str.find_last_of('.');
        if (pos != std::string::npos)
        {
            auto extension = str.substr(pos);

            if (extension == ".dcm")
            {
                using convert_t = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_t, wchar_t> strconverter;
                CreateDataFromDicom(strconverter.from_bytes(str));
                return "Assets/VolumetricData/DICOM.dat";
            }
        }

        return str;
    }
}

UiView3D::UiView3D(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiRenderArea(name, position, size, true),
    m_scene("Scene 3D")
{
    SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
    Init();
}

void UiView3D::SetTransferFunction(const NotOwner<TransferFunctionRenderer>& transferFunction)
{
    m_transferFunction = transferFunction;

    auto cubeMarch = static_cast<MarchCubeRenderer*>(m_scene.GetNodeByName("Cube march"));
    cubeMarch->SetTransferTexture(m_transferFunction->GetTransferFunctionTexture());
}

void UiView3D::Init()
{
    auto root = m_scene.GetRootNode();

    //Camera
    auto camera = new Camera("Main camera", &m_scene, UiRenderArea::GetSize());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->CalculateProjectionMatrix();
    camera->CalculateViewMatrix();

    m_scene.SetMainCamera(camera);
    root->AddNode(camera);

    //Light
    auto dirLight = new DirectionalLight("dirLight", &m_scene);
    dirLight->SetDirection(glm::vec3(1.0f, -1.0f, 0.0f));
    dirLight->SetAmbientStrength(0.15f);
    dirLight->SetLightColor(glm::vec3(0.9f, 0.9f, 0.9f));

    root->AddNode(dirLight);

    auto cubeMarch = new MarchCubeRenderer("Cube march", &m_scene);
    root->AddNode(cubeMarch);
}

void UiView3D::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::WindowResize:
    {
         auto cameraNode = m_scene.GetNodeByName("Main camera");
        
        if(cameraNode != nullptr)
        {
            auto camera = static_cast<Camera*>(cameraNode);
            camera->SetRenderAreaSize(UiRenderArea::GetSize());
        }
    }
    break;
    
    case EventType::MouseMove:
    case EventType::MouseDown:
    case EventType::MouseUp:
    case EventType::MouseScroll:
    {
        if(!PointInStaticRect<glm::ivec2>(Application::GetMouse().GetMousePosition(), GetPosition(), GetSize()) && !m_window->IsCursorLocked())
            return;
    }
    break;
    case EventType::GradientUpdated:
    {
        m_scene.SetUpdated(true);
    }
    break;

    default:
        break;
    }

    m_scene.HandleEvent(event);
    UiRenderArea::HandleEvent(event);
}

void UiView3D::Update()
{
    m_scene.Update();

    if(m_scene.IsUpdated())
    {
        m_areaUpdated = true;
        m_scene.SetUpdated(false);
    }

    UiRenderArea::Update();
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
    m_areaUpdated = true;
    UiRenderArea::Render();
}