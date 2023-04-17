#include "Ullpch.h"
#include "UiMenuView.h"
#include "Core/PlatformDependantFreeFunctions.h"
#include "Rendering/IconsCode/IconCodes.h"
#include "Logger/Logger.h"
#include "Event/EventAggregator.h"
#include <limits>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

namespace
{
    constexpr int maxUint16 = (1 << 16) - 1;
}

UiMenuView::UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiArea(name, position, size, false)
    //m_gradientEditor("GradientEditor", glm::uvec2(size.x * 0.5f, 80), glm::uvec2(size.x * 0.8f, 50))
{
    SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    
    m_transferFunction.AddPoint(TransferPoint{glm::vec3(1.0f, 1.0f, 1.0f), 0});
    m_transferFunction.AddPoint(TransferPoint{glm::vec3(1.0f, 0.0f, 1.0f), 100});
    m_transferFunction.AddPoint(TransferPoint{glm::vec3(0.0f, 0.0f, 0.0f), 200});
    m_transferFunction.GenerateTransferFunction();

    //m_gradientEditor.SetViewSize(size);
    //m_gradientEditor.SetViewPos(position);
    //m_gradientEditor.SetTransferFunction(&m_transferFunction);
    //m_gradientEditor.CreateResources();

    Init();   
}


void UiMenuView::HandleEvent(Event* event)
{
    switch (event->GetType())
    {
    case EventType::FileLoaded:
        m_newDataLoaded = true;
    break;
    
    case EventType::ExaminationThresholdChanged:
        if(m_newDataLoaded)
        {
            auto newThresholds = static_cast<ExaminationThresholdChangedEvent*>(event)->GetVal();
            m_cubeMarchTresholds.x = newThresholds.x;
            m_cubeMarchTresholds.y = newThresholds.y;
            m_newDataLoaded = false;
        }
    case EventType::WindowResize:
    {
        //m_gradientEditor.SetViewSize(m_size);
        //m_gradientEditor.SetViewPos(m_position);
        //m_gradientEditor.CreateResources();
    }
    break;

    break;
    }

    //if(m_renderTransferEditor)
      //  m_gradientEditor.HandleEvent(event);
}

void UiMenuView::Update()
{
    m_areaUpdated = true;

    //if(m_renderTransferEditor)
      //  m_gradientEditor.Update();
}

void UiMenuView::Render()
{
    if(m_areaUpdated)
    {
        m_frameBuffer->Bind();
        RenderBackground();
        //if(m_renderTransferEditor)
          //  m_gradientEditor.Render();
        m_frameBuffer->Unbind();

        m_areaUpdated = false;
    }


	RenderUI();
}

void UiMenuView::Init()
{
    m_cubeMarchTresholds.x = 0;
    m_cubeMarchTresholds.y = maxUint16;

    m_cuttingSettings.cuttingPositions = glm::vec3(100.0f, 100.0f, 100.0f);
    m_cuttingSettings.invertedAxis = { false, false, false };
}

void UiMenuView::RenderUI()
{
    /*
    ImGui::SetNextWindowSize(ImVec2((float)m_size.x, (float)m_size.y));
	ImGui::SetNextWindowPos(ImVec2((float)m_position.x, (float)m_position.y));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin("MenuViewImGui", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleVar();

    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.0, 0.0, 0.0, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.f);

    ImGuiStyle& style = ImGui::GetStyle();
    auto orignalFramePadding = style.FramePadding;
    auto orignalInterSpacing = style.ItemInnerSpacing;

    style.FramePadding = ImVec2(12.0f, 12.0f);
    style.ItemInnerSpacing = ImVec2(0.0f, orignalInterSpacing.y);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));

        style.FramePadding = ImVec2(12, 12);

        m_renderTransferEditor = false;

        if (ImGui::BeginTabItem(ICON_LOAD))
        {
            RenderLoadTab();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_CUT_PLANE))
        {
            RenderCutSettings();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(ICON_TRANSFER))
        {
            RenderTransferFunctionSettings();
            ImGui::EndTabItem();
            m_renderTransferEditor = true;
        }
        if (ImGui::BeginTabItem(ICON_SETTINGS_1))
        {
            RenderDataSettings();
            ImGui::EndTabItem();
        }

        style.FramePadding = orignalFramePadding;
        style.ItemInnerSpacing = orignalInterSpacing;
        ImGui::EndTabBar();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::End();
    */
}

void UiMenuView::RenderLoadTab()
{
    /*
    //Load file button
    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    if (ImGui::Button("Load file", ImVec2((float)m_size.x - 40.0f, 32.0f)))
    {
        m_filePath = CreateFileOpenDialog(FileExtentions::DAT | FileExtentions::DICOM);

        if (m_filePath.has_value())
        {
            std::string path(m_filePath.value().begin(), m_filePath.value().end());
            EventAggregator::Publish(std::make_shared<DataLoadEvent>(EventType::FileLoaded, path));
        }
        m_firstDataLoaded = true;
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    //Text under button
    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    if (m_filePath.has_value())
    {
        std::wstring pathW = m_filePath.value();
        std::string path(pathW.begin(), pathW.end());
        
        auto pos = pathW.find_last_of(L"\\");
        if (pos != std::string::npos)
        {
            std::wstring filenameW = pathW.substr(pos + 1, pathW.size());
            std::string filename(filenameW.begin(), filenameW.end());
            std::string fileText;

            fileText = "File loaded: " + filename;

            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            auto startClipRect = ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
            auto endClipRect = ImVec2(startClipRect.x + m_size.x - 40.0f, (float)m_size.y);

            ImGui::PushClipRect(startClipRect, endClipRect, false);
            ImGui::TextColored(ImVec4(0.38f, 0.94f, 0.47f, 1.0f), fileText.c_str());

            //Tool-tip
            if (ImGui::IsItemHovered())
            {
                auto& style = ImGui::GetStyle();
                auto originalWindowPadding = style.WindowPadding;
                style.WindowPadding = ImVec2(3.0f, 3.0f);

                ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 2.0f);
                ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

                ImGui::SetTooltip(fileText.c_str());

                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                style.WindowPadding = originalWindowPadding;
            }
            ImGui::PopClipRect();
        }
        else
            ULOGE("File was recognized but not loaded");
    }
    else
    {
        if(m_firstDataLoaded)
            ImGui::TextColored(ImVec4(0.86f, 0.2f, 0.31f, 1.0f), "No file loaded.");
    }
    */
}

void UiMenuView::RenderDataSettings()
{
    /*
    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Text("Examination thresholds:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    auto& style = ImGui::GetStyle();
    auto orignaItemlInnerSpacing = style.ItemInnerSpacing;
    auto orginalFramePadding = style.FramePadding;

    style.ItemInnerSpacing.x = 10.0f;
    style.FramePadding = ImVec2(5.0f, 5.0f);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    auto originalThresholds = m_cubeMarchTresholds;
    ImGui::DragIntRange2("##Thresholds", &m_cubeMarchTresholds.x, &m_cubeMarchTresholds.y, 1, 1, maxUint16, "Min: %d", "Max: %d");

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    style.ItemInnerSpacing = orignaItemlInnerSpacing;
    style.FramePadding = orginalFramePadding;

    if (m_cubeMarchTresholds != originalThresholds && m_cubeMarchTresholds.x <= m_cubeMarchTresholds.y)
    {
        EventAggregator::Publish(std::make_shared<ExaminationThresholdChangedEvent>(EventType::ExaminationThresholdChanged, glm::uvec2(m_cubeMarchTresholds.x, m_cubeMarchTresholds.y)));
    }
    */
}

void UiMenuView::RenderCutSettings()
{
    /*
    auto& style = ImGui::GetStyle();

    auto orginalFramePadding = style.FramePadding;
    style.FramePadding = ImVec2(0.0f, 4.0f);

    auto orginalCuttingSettings = m_cuttingSettings;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(10.0f, 0.0f));

    //Start
    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Text("Cutting settings:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    //Cut X
    ImGui::Text("Cut X:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Checkbox("Invert##X", &m_cuttingSettings.invertedAxis[0]);

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.86f, 0.2f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.95f, 0.31f, 0.43f, 1.0f));

    ImGui::SliderFloat("##CutX", &m_cuttingSettings.cuttingPositions.x, 0, 100, "%.2f %", ImGuiSliderFlags_None);

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    //Cut Y
    ImGui::Text("Cut Y:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Checkbox("Invert##Y", &m_cuttingSettings.invertedAxis[1]);

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.32f, 0.75f, 0.02f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.49f, 1.0f, 0.12f, 1.0f));

    ImGui::SliderFloat("##CutY", &m_cuttingSettings.cuttingPositions.y, 0, 100, "%.2f %", ImGuiSliderFlags_None);

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    //Cut Z
    ImGui::Text("Cut Z:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Checkbox("Invert##Z", &m_cuttingSettings.invertedAxis[2]);

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.02f, 0.51f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.02f, 0.69f, 0.95f, 1.0f));

    ImGui::SliderFloat("##CutZ", &m_cuttingSettings.cuttingPositions.z, 0, 100, "%.2f", ImGuiSliderFlags_None);

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    //End
    style.FramePadding = orginalFramePadding;

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    if(orginalCuttingSettings != m_cuttingSettings)
        EventAggregator::Publish(std::make_shared<CuttingSettingsChangedEvent>(EventType::CuttingSettingsChanged, m_cuttingSettings));
        */
}

void UiMenuView::RenderTransferFunctionSettings()
{
    /*
    auto& style = ImGui::GetStyle();
    
    auto orginalFramePadding = style.FramePadding;
    style.FramePadding = ImVec2(0.0f, 4.0f);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.251f, 0.251f, 0.251f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 12);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(10.0f, 0.0f));

    //Start
    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

    ImGui::Text("Transfer settings:");

    ImGui::SetCursorPosX(20);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    style.FramePadding = orginalFramePadding;
    */
}