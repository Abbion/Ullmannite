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
    UiRenderArea(name, position, size, true)
{
    SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
    Init();
}

void UiView3D::Init()
{
    m_sceneview = std::make_shared<UiSceneView>("Scene view 3D");
    AddChildNode(m_sceneview);

    auto& scene = m_sceneview->GetScene();
    auto root = scene.GetRootNode();

    //Camera
    auto camera = new Camera("main camera", &scene, UiRenderArea::GetSize());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera->CalculateProjectionMatrix();
    camera->CalculateViewMatrix();

    scene.SetMainCamera(camera);
    root->AddNode(camera);

    //Light
    auto dirLight = new DirectionalLight("dirLight", &scene);
    dirLight->SetDirection(glm::vec3(1.0f, -1.0f, 0.0f));
    dirLight->SetAmbientStrength(0.15f);
    dirLight->SetLightColor(glm::vec3(0.9f, 0.9f, 0.9f));

    root->AddNode(dirLight);
}

void UiView3D::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::WindowResize:
    {
        auto& scene = m_sceneview->GetScene();
        auto cameraNode = scene.GetNodeByName("main camera");
        
        if(cameraNode != nullptr)
        {
            auto camera = static_cast<Camera*>(cameraNode);
            camera->SetRenderAreaSize(UiRenderArea::GetSize());
        }
    }
    break;

    case EventType::KeyDown:
    {
        const auto key = static_cast<KeyDownEvent*>(event)->GetVal();
        if (key == Keyboard::Key::L_SHFT)
        {
            auto& scene = m_sceneview->GetScene();
            auto cameraNode = scene.GetNodeByName("main camera");
            auto camera = static_cast<Camera*>(cameraNode);

            auto directionalLightNode = scene.GetNodeByName("dirLight");
            auto directionalLight = static_cast<DirectionalLight*>(directionalLightNode);

            directionalLight->SetDirection(camera->GetForward());
        }
    }
    break;
    
    case EventType::MouseMove:
    case EventType::MouseDown:
    case EventType::MouseUp:
    case EventType::MouseScroll:
    {
        auto& window = Application::GetWindow();
        if(!PointInStaticRect<glm::ivec2>(Application::GetMouse().GetMousePosition(), GetPosition(), GetSize()) && !window.IsCursorLocked())
            return;
    }
    break;
    case EventType::TransferFunctionUpdated:
    {
        auto& scene = m_sceneview->GetScene();
        auto cubeMarchNode = scene.GetNodeByName("march cube");

        if (cubeMarchNode == nullptr)
            return;

        auto cubeMarch = static_cast<MarchCubeRenderer*>(cubeMarchNode);

        const auto colorTransferNode = GetRoot()->GetNodeByName("colorTransferLinearGradient");
        if (colorTransferNode)
        {
            const auto colorTransfer = static_cast<UiLinearColorGradient*>(colorTransferNode.value().Get());
            const auto transferPoints = colorTransfer->GetGradientColors();
            m_transferFunction = std::make_unique<TransferFunctionRenderer>(transferPoints);
            m_transferFunction->GenerateTransferFunction();
            cubeMarch->SetTransferFunction(m_transferFunction->GetTransferFunctionTexture());
        }
    }
    break;
    case EventType::VolumeLoaded:
    {
        auto& scene = m_sceneview->GetScene();
        auto root = scene.GetRootNode();
        auto cubeMarch = new MarchCubeRenderer("march cube", &scene);
        root->AddNode(cubeMarch);

        const auto colorTransferNode = GetRoot()->GetNodeByName("colorTransferLinearGradient");
        if (colorTransferNode)
        {
            const auto colorTransfer = static_cast<UiLinearColorGradient*>(colorTransferNode.value().Get());
            const auto transferPoints = colorTransfer->GetGradientColors();
            m_transferFunction = std::make_unique<TransferFunctionRenderer>(transferPoints);
            m_transferFunction->GenerateTransferFunction();
            cubeMarch->SetTransferFunction(m_transferFunction->GetTransferFunctionTexture());
        }
    }
    break;

    default:
        break;
    }

    UiRenderArea::HandleEvent(event);
}

void UiView3D::Update()
{
    if (const auto volumeThresholdSliderNode = GetRoot()->GetNodeByName("volumeThresholdSlider"))
    {
        auto& scene = m_sceneview->GetScene();
        if (auto marchCubeRendererNode = scene.GetNodeByName("march cube"))
        {
            auto marchCubeRenderer = static_cast<MarchCubeRenderer*>(marchCubeRendererNode);

            const auto volumeThresholdSlider = static_cast<UiTwoSideSlider*>(volumeThresholdSliderNode->Get());
            const auto minValue = volumeThresholdSlider->GetMinSliderValue();
            const auto maxValue = volumeThresholdSlider->GetMaxSliderValue();

            marchCubeRenderer->SetThresholdValues(static_cast<glm::uint>(std::round(minValue)), static_cast<glm::uint>(std::round(maxValue)));
        }

    }


    UiRenderArea::Update();
}

void UiView3D::Render()
{
    Application::GetRenderer().SetFaceCulling(Renderer::FaceCulling::BACK);
    Application::GetRenderer().SetDepth(Renderer::State::ENABLE);

    m_frameBuffer->Bind();
    UiRenderArea::Render();
    m_frameBuffer->Unbind();

    Application::GetRenderer().SetDepth(Renderer::State::DISABLE);
    Application::GetRenderer().SetFaceCulling(Renderer::FaceCulling::NONE);
}