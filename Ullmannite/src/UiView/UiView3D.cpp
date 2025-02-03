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

#include "DataLoaders/VolumeLoader.h"
#include "DataStructures/VolumeData.h"

#include <algorithm>
#include "Core/PlatformDependantFreeFunctions.h"
#include <codecvt>
#include <locale>

using namespace Ull;

namespace {
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
    m_scene("Scene 3D"),
    m_titleText{ std::make_shared<UiText>("testText", glm::uvec2(100, 100), glm::uvec2(size.y, size.y),
     L"ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n\n\nUllmanite") },
    m_titleButton{ std::make_shared<UiButton>("testButtonView3d", glm::uvec2(100, 100), glm::uvec2(100, 100))},
    m_colorGradient{ std::make_shared<UiLinearColorGradient>("testColorGradient", glm::uvec2(100, 300), glm::uvec2(300, 50), Ull::UiLinearColorGradient::GradientDirection::HORIZONTAL) },
    m_colorSpace{ std::make_shared<UiSpace>("colorSpace", glm::vec2(100, 400), glm::vec2(50, 50)) },
    m_rectGradient{ std::make_shared<UiRectGradient>("rectGradinet", glm::vec2(200, 400), glm::vec2(200, 250)) }
{
    SetBackgroundColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));

    //std::wstring aa = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n\n\nUllmanite";
    std::wstring aa = L"File loaded";
    m_titleButton->GetTextControl()->SetString(aa);
    m_titleButton->GetTextControl()->SetSmoothingExceptance({ L'i', L'l' });

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

    //m_titleText->CreateResources();
    m_titleButton->CreateResources();
    //AddChildNode(m_titleText);
    AddChildNode(m_titleButton);

    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.166f,   glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.333f,   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.5f,     glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.666f,   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 0.833f,   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) });
    m_colorGradient->AddColor(UiLinearColorGradient::GradientColorData{ 1.0f,     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) });
    m_colorGradient->SetInteractive(true);

    m_colorGradient->CreateResources();
    AddChildNode(m_colorGradient);

    m_colorSpace->CreateResources();
    AddChildNode(m_colorSpace);

    m_rectGradient->CreateResources();
    AddChildNode(m_rectGradient);
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

    case EventType::FileLoaded:
    {
        auto path = static_cast<DataLoadEvent*>(event)->GetVal();
        path = ConvertDcmToDat(path);
        auto dataSet = LoadVolumeData(path);
        auto cubeMarchRenderer = m_scene.GetNodeByName("Cube march");
        if (cubeMarchRenderer != nullptr)
        {
            auto renderer = static_cast<MarchCubeRenderer*>(cubeMarchRenderer);
            renderer->SetVolumeData(dataSet);
            renderer->GenerateMesh();
            m_scene.SetUpdated(true);
        }
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

    const auto color = m_colorGradient->GetColorFromLastInteraction();
    m_colorSpace->SetBackgroundColor(color);

    const auto hslColor = RgbToHsv(color);

    m_rectGradient->SetColorData({ UiRectGradient::GradientColorData{ 0, glm::vec3(hslColor.x, 0.0f, 1.0f) } ,
                                UiRectGradient::GradientColorData{ 1, glm::vec3(hslColor.x, 1.0f, 1.0f) },
                                UiRectGradient::GradientColorData{ 2, glm::vec3(hslColor.x, 0.0f, 0.0f) },
                                UiRectGradient::GradientColorData{ 3, glm::vec3(hslColor.x, 1.0f, 0.0f) } });

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

#include <imgui.h>
void UiView3D::Render()
{
    ImGui::Text("BoxSettings");

    m_titleButton->SetBackgroundColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

    auto boxPosition = m_titleButton->GetPosition();
    auto boxSize = m_titleButton->GetSize();

    ImGui::SliderFloat2("box position", (float*)&boxPosition, -100.0f, 1500.0f);
    ImGui::SliderFloat2("box size", (float*)&boxSize, 1.0f, 1000.0f);

    m_titleButton->SetPosition(boxPosition);
    m_titleButton->SetSize(boxSize);

    auto textControll = m_titleButton->GetTextControl();

    auto fontSize = static_cast<int>(textControll->GetFontSize());
    auto spaceing = textControll->GetSpaceing();
    auto leading = textControll->GetLeading();
    auto smoothing = textControll->GetEdgeSmoothing();
    auto threshold = textControll->GetSampleThreshold();
    auto color = textControll->GetColor();
    auto aligment = textControll->GetAlignment();

    ImGui::Text("Text settings");

    ImGui::SliderInt("fontSize", &fontSize, 1, 256);
    ImGui::SliderFloat("spaceing", &spaceing, -10.0f, 100.0f);
    ImGui::SliderFloat("leading", &leading, -2.0f, 5.0f);
    ImGui::SliderFloat("smoothing", &smoothing, 0.0f, 20.0f);
    ImGui::SliderFloat("threshold", &threshold, -10.0f, 50.0f);
    ImGui::SliderFloat4("color", (float*)&color, 0.0f, 1.0f);

    const char* horizontalAligmentList[] = { "Left", "Center", "Right" };
    static int current_h_aligment = 1;
    ImGui::ListBox("horizontal aligment", &current_h_aligment, horizontalAligmentList, 3);

    if (current_h_aligment == 0)
        textControll->SetAlignment(UiText::HorizontalAlignment::LEFT, aligment.vertical);

    if (current_h_aligment == 1)
        textControll->SetAlignment(UiText::HorizontalAlignment::CENTER, aligment.vertical);

    if (current_h_aligment == 2)
        textControll->SetAlignment(UiText::HorizontalAlignment::RIGHT, aligment.vertical);

    aligment = textControll->GetAlignment();

    const char* verticalAligmentList[] = { "Top", "Center", "Bottom" };
    static int current_v_aligment = 1;
    ImGui::ListBox("vertical aligment", &current_v_aligment, verticalAligmentList, 3);

    if (current_v_aligment == 0)
        textControll->SetAlignment(aligment.horizontal, UiText::VerticalAlignment::TOP);

    if (current_v_aligment == 1)
        textControll->SetAlignment(aligment.horizontal, UiText::VerticalAlignment::CENTER);

    if (current_v_aligment == 2)
        textControll->SetAlignment(aligment.horizontal, UiText::VerticalAlignment::BOTTOM);

    textControll->SetFontSize(static_cast<unsigned>(fontSize));
    textControll->SetSpaceing(spaceing);
    textControll->SetLeading(leading);
    textControll->SetEdgeSmoothing(smoothing);
    textControll->SetSampleThreshold(threshold);
    textControll->SetColor(color);
    textControll->CreateResources();
    textControll->SetBackgroundColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    textControll->Update();

    m_areaUpdated = true;

    //if(m_areaUpdated)
    {
        m_frameBuffer->Bind();

		RenderBackground();

        UiRenderArea::Render();

        Application::GetRenderer().SetDepth(Renderer::State::ENABLE);
        Application::GetRenderer().Clear(Renderer::ClearBits::DEPTH);

        //Application::GetRenderer().SetFaceCulling(Renderer::FaceCulling::BACK);
        //Application::GetRenderer().SetFaceWinding(Renderer::FaceWinding::COUNTER_CLOCKWISE);
        m_scene.Render();
        //Application::GetRenderer().SetFaceCulling(Renderer::FaceCulling::NONE);
        //Application::GetRenderer().SetDepth(Renderer::State::DISABLE);

		m_frameBuffer->Unbind();

		m_areaUpdated = false;
    }

}