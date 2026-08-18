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
    case EventType::DataFolderSelected:
    {
        auto& scene = m_sceneview->GetScene();
        auto marchCubeNode = scene.GetNodeByName("march cube");
        if (marchCubeNode)
        {
            scene.GetRootNode()->RemoveNode(marchCubeNode);
            delete marchCubeNode;
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
    auto& scene = m_sceneview->GetScene();

    if (auto marchCubeRendererNode = scene.GetNodeByName("march cube"))
    {
        auto marchCubeRenderer = static_cast<MarchCubeRenderer*>(marchCubeRendererNode);

        if (const auto volumeThresholdSliderNode = GetRoot()->GetNodeByName("volumeThresholdSlider"))
        {
            const auto volumeThresholdSlider = static_cast<UiTwoSideSlider*>(volumeThresholdSliderNode->Get());
            const auto minValue = volumeThresholdSlider->GetMinSliderValue();
            const auto maxValue = volumeThresholdSlider->GetMaxSliderValue();

            marchCubeRenderer->SetThresholdValues(static_cast<glm::uint>(std::round(minValue)), static_cast<glm::uint>(std::round(maxValue)));
        }

        if (const auto colorTransferNode = GetRoot()->GetNodeByName("colorTransferLinearGradient"))
        {
            const auto colorTransfer = static_cast<UiLinearColorGradient*>(colorTransferNode.value().Get());
            const auto transferPoints = colorTransfer->GetGradientColors();
            m_transferFunction = std::make_unique<TransferFunctionRenderer>(transferPoints);
            m_transferFunction->GenerateTransferFunction();
            marchCubeRenderer->SetTransferFunction(m_transferFunction->GetTransferFunctionTexture());
        }
    }

    UiRenderArea::Update();
}

void UiView3D::Render()
{
    Application::GetRenderer().SetDepth(Renderer::State::ENABLE);

    m_frameBuffer->Bind();
    UiRenderArea::Render();
    m_frameBuffer->Unbind();

    Application::GetRenderer().SetDepth(Renderer::State::DISABLE);
    Application::GetRenderer().SetFaceCulling(Renderer::FaceCulling::NONE);
}