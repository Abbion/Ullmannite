#include "Ullpch.h"
#include "UiMenuView.h"
#include <Application/Application.h>
#include "Core/PlatformDependantFreeFunctions.h"
#include "Logger/Logger.h"
#include "Resources/Fonts/IconCodes.h"
#include <limits>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

using namespace Ull;

namespace
{
    constexpr int maxUint16 = (1 << 16) - 1;
    constexpr unsigned TOOL_BAR_SIZE = 6;
    constexpr unsigned TOOL_TAB_ICON_SIZE = 24;
    constexpr unsigned MENU_TEXT_SIZE = 14;
    constexpr float MARKER_SIZE = 10.0f;
    constexpr float BOTTOM_THRESHILD_DEFAULT_RATE = 0.25f;
    constexpr float TOP_THRESHILD_DEFAULT_RATE = 0.8f;
}

UiMenuView::UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiRenderArea(name, position, size, false)
{
    SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    
    CreateControls();

    m_cuttingSettings.cuttingPositions = glm::vec3(0.0f, 0.0f, 0.0f);
    m_cuttingSettings.invertedAxis = { false, false, false };
}

void UiMenuView::HandleEvent(Event* event)
{
    switch (event->GetType())
    {    
    case EventType::MouseMove:
    {
        if (m_transferLinearGradient->IsVisible())
        {
            bool mouseOverMarker = false;
            for (auto marker : m_transferMarkers)
            {
                if (marker->IsHover())
                {
                    mouseOverMarker = true;
                    break;
                }
            }

            if (mouseOverMarker)
            {
                const auto mousePosition = glm::vec2(Application::GetMouse().GetMousePosition());
                m_cursorIndicatorText->SetPosition(mousePosition + glm::vec2(16.0f, 5.0f) - GetGlobalPosition());
                m_cursorIndicatorText->SetVisibility(true);
            }
            else
            {
                m_cursorIndicatorText->SetVisibility(false);
            }
        }
        else
        {
            m_cursorIndicatorText->SetVisibility(false);
        }
    }
    break;

    case EventType::MouseDoubleUp:
    {
        if (m_transferLinearGradient->IsVisible() == false)
            break;

        if (m_transferLinearGradient->IsHover())
        {
            const auto mousePosition = Application::GetMouse().GetMousePosition();
            const auto linearGradientPosition = m_transferLinearGradient->GetPosition();
            const auto linearGradientSize = m_transferLinearGradient->GetSize();
            const auto normalizedPosition = (mousePosition.x - linearGradientPosition.x) / linearGradientSize.x;
            const auto initColor = m_transferLinearGradient->GetColorForRatio(normalizedPosition);

            m_transferLinearGradient->AddColor(GradientColorData{ normalizedPosition, initColor });
            m_transferLinearGradient->CreateResources();
            CreateMarkersForTransferFunction();
            event->MarkHandeled(true);
        }

        for (auto marker : m_transferMarkers)
        {
            if (marker->IsHover())
            {
                m_cursorIndicatorText->SetVisibility(false);
                break;
            }
        }
    }
    break;

    case EventType::MouseUp:
    {
        const auto isRightPressed = reinterpret_cast<MouseDoubleUp*>(event)->GetVal() == Mouse::Button::RIGHT;

        if (isRightPressed == false || m_transferMarkers.size() < 3)
            break;

        size_t itr = 0;
        for (auto marker : m_transferMarkers)
        {
            if (marker->IsHover())
            {
                RemoveChildNode(marker);
                m_transferMarkers.erase(m_transferMarkers.begin() + itr);
                m_cursorIndicatorText->SetVisibility(false);
                break;
            }

            itr++;
        }
    }
    break;

    case EventType::VolumeLoaded:
    {
        const auto& volume = Application::GetResourceManager().GetVolumeManager().GetVolume();
        m_loadFileText->SetString(std::format(L"Loaded files from {}", volume.name).c_str());
        m_loadFileText->SetColor(glm::vec4(0.33f, 0.58f, 0.4f, 1.0f));
        m_newDataLoaded = true;

        m_minExaminationThresholdValue->SetString(std::format(L"{}", volume.minValue));
        m_maxExaminationThresholdValue->SetString(std::format(L"{}", volume.maxValue));

        m_thresholdSlider->SetMinLimitValue(volume.minValue);
        m_thresholdSlider->SetMaxLimitValue(volume.maxValue);

        const auto valueRange = volume.maxValue - volume.minValue;
        m_thresholdSlider->SetMaxValue(volume.minValue + TOP_THRESHILD_DEFAULT_RATE * valueRange);
        m_thresholdSlider->SetMinValue(volume.minValue + BOTTOM_THRESHILD_DEFAULT_RATE * valueRange);
    }
    break;
    }

	UiRenderArea::HandleEvent(event);
}

void UiMenuView::Update()
{
    if (m_toolTabs[ToolTypes::Transfer]->IsEnabled())
    {
        m_transferLinearGradient->ClearColorData();

        for (auto marker : m_transferMarkers)
        {
            const auto colorRatio = marker->GetPositionRatio();
            const auto color = marker->GetColor();
            m_transferLinearGradient->AddColor(GradientColorData{ colorRatio, color });
        }

        m_transferLinearGradient->CreateResources();
    }
    else if (m_toolTabs[ToolTypes::Cut]->IsEnabled())
    {
        auto cuttingSettingDirty = false;

        const auto cutX = m_cutSettingsCutxSlider->GetValue();
        const auto cutY = m_cutSettingsCutySlider->GetValue();
        const auto cutZ = m_cutSettingsCutzSlider->GetValue();

        const auto invertCutX = m_cutSettingsCutxInvertToggle->IsEnabled();
        const auto invertCutY = m_cutSettingsCutyInvertToggle->IsEnabled();
        const auto invertCutZ = m_cutSettingsCutzInvertToggle->IsEnabled();

        const auto compareAndMarkDirty = [&cuttingSettingDirty]<typename T>(T& lparam, const T rparam) {
            if (lparam != rparam)
            {
                lparam = rparam;
                cuttingSettingDirty = true;
            }
        };

        compareAndMarkDirty(m_cuttingSettings.cuttingPositions.x, cutX);
        compareAndMarkDirty(m_cuttingSettings.cuttingPositions.y, cutY);
        compareAndMarkDirty(m_cuttingSettings.cuttingPositions.z, cutZ);

        compareAndMarkDirty(m_cuttingSettings.invertedAxis[0], invertCutX);
        compareAndMarkDirty(m_cuttingSettings.invertedAxis[1], invertCutY);
        compareAndMarkDirty(m_cuttingSettings.invertedAxis[2], invertCutZ);

        Application::GetEventQueue().PushEvent(std::make_shared<CuttingSettingsChangedEvent>(EventType::CuttingSettingsChanged, m_cuttingSettings));
    }

	UiRenderArea::Update();
}

void UiMenuView::Render()
{
    UiRenderArea::Render();
}

void UiMenuView::CreateControls()
{
    CreateLoadPanel();
    CreateCutPanel();
    CreateColorTransformPanel();
    CreateSettingsPanel();

    // Tool tabs
    static auto toogleIsEnabledManageTabs = [this](UiToggle& toggleElement) {
        toggleElement.SetBackgroundColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        toggleElement.SetHoverColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

        for (auto& [_, toolTab] : m_toolTabs)
        {
            if (toolTab->GetId() == toggleElement.GetId())
                continue;

            toolTab->SetEnabled(false);
        }
    };

    static auto toogleIsDisabledManageTabs = [this](UiToggle& toggleElement) {
        toggleElement.SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
        toggleElement.SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

        auto isAnyTabEnabled = false;
        for (auto& [_, toolTab] : m_toolTabs) 
        {
            if(toolTab->IsEnabled() == true)
                isAnyTabEnabled = true;
        }

        if (!isAnyTabEnabled)
            toggleElement.SetEnabled(true);
    };

    auto loadTab = std::make_shared<UiToggle>("menuLoadTab", glm::uvec2(0, 0), glm::uvec2(43, 45));
    loadTab->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    loadTab->SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    loadTab->CreateResources();

    loadTab->SetOnEnabledFunction([this](UiToggle& toggleElement) {
        m_loadFileButton->SetVisibility(true);
        m_loadFileText->SetVisibility(true);

        m_cutSettingsText->SetVisibility(false);
        m_cutSettingsCutxText->SetVisibility(false);
        m_cutSettingsCutxInvertToggle->SetVisibility(false);
        m_cutSettingsCutxInvertText->SetVisibility(false);
        m_cutSettingsCutxSlider->SetVisibility(false);
        m_cutSettingsCutyText->SetVisibility(false);
        m_cutSettingsCutyInvertToggle->SetVisibility(false);
        m_cutSettingsCutyInvertText->SetVisibility(false);
        m_cutSettingsCutySlider->SetVisibility(false); 
        m_cutSettingsCutzText->SetVisibility(false);
        m_cutSettingsCutzInvertToggle->SetVisibility(false);
        m_cutSettingsCutzInvertText->SetVisibility(false);
        m_cutSettingsCutzSlider->SetVisibility(false);

        m_transferSettingsText->SetVisibility(false);
        m_transferLinearGradient->SetVisibility(false);
        for (auto& marker : m_transferMarkers)
            marker->SetVisibility(false);

        m_examinationThresholdSettingsText->SetVisibility(false);
        m_maxExaminationThresholdText->SetVisibility(false);
        m_maxExaminationThresholdValue->SetVisibility(false);
        m_minExaminationThresholdText->SetVisibility(false);
        m_minExaminationThresholdValue->SetVisibility(false);
        m_thresholdSlider->SetVisibility(false);
        
        toogleIsEnabledManageTabs(toggleElement);
       });

    loadTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    loadTab->SetEnabled(true);
    loadTab->Update();

    auto loadTabText = loadTab->GetTextControl();
	loadTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	loadTabText->SetFont(FontTag::UI_ICON);
    loadTabText->SetSampleThreshold(-2);
    loadTabText->SetEdgeSmoothing(5);
	loadTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::LOAD_FILE) });

    AddChildNode(loadTab);
    m_toolTabs.insert(std::pair(ToolTypes::Load, std::move(loadTab)));

    auto cutTab = std::make_shared<UiToggle>("menuCutTab", glm::uvec2(0, 0), glm::uvec2(1, 1)); 
    cutTab->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    cutTab->SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    cutTab->CreateResources();

    cutTab->SetOnEnabledFunction([this](UiToggle& toggleElement){
        m_loadFileButton->SetVisibility(false);
        m_loadFileText->SetVisibility(false);

        m_cutSettingsText->SetVisibility(true);
        m_cutSettingsCutxText->SetVisibility(true);
        m_cutSettingsCutxInvertToggle->SetVisibility(true);
        m_cutSettingsCutxInvertText->SetVisibility(true);
        m_cutSettingsCutxSlider->SetVisibility(true);
        m_cutSettingsCutyText->SetVisibility(true);
        m_cutSettingsCutyInvertToggle->SetVisibility(true);
        m_cutSettingsCutyInvertText->SetVisibility(true);
        m_cutSettingsCutySlider->SetVisibility(true);
        m_cutSettingsCutzText->SetVisibility(true);
        m_cutSettingsCutzInvertToggle->SetVisibility(true);
        m_cutSettingsCutzInvertText->SetVisibility(true);
        m_cutSettingsCutzSlider->SetVisibility(true);

        m_transferSettingsText->SetVisibility(false);
        m_transferLinearGradient->SetVisibility(false);
        for (auto& marker : m_transferMarkers)
            marker->SetVisibility(false);

        m_examinationThresholdSettingsText->SetVisibility(false);
        m_maxExaminationThresholdText->SetVisibility(false);
        m_maxExaminationThresholdValue->SetVisibility(false);
        m_minExaminationThresholdText->SetVisibility(false);
        m_minExaminationThresholdValue->SetVisibility(false);
        m_thresholdSlider->SetVisibility(false);

        toogleIsEnabledManageTabs(toggleElement);
    });
    cutTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    cutTab->Update();

    auto cutTabText = cutTab->GetTextControl();
	cutTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	cutTabText->SetFont(FontTag::UI_ICON);
    cutTabText->SetSampleThreshold(-2);
    cutTabText->SetEdgeSmoothing(5);
	cutTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CUTING_SETTINGS) });

    AddChildNode(cutTab);
    m_toolTabs.insert(std::pair(ToolTypes::Cut, std::move(cutTab)));

    auto transferTab = std::make_shared<UiToggle>("transferCutTab", glm::uvec2(0, 0), glm::uvec2(1, 1)); 
    transferTab->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    transferTab->SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    transferTab->CreateResources();

    transferTab->SetOnEnabledFunction([this](UiToggle& toggleElement){
        m_loadFileButton->SetVisibility(false);
        m_loadFileText->SetVisibility(false);

        m_cutSettingsText->SetVisibility(false);
        m_cutSettingsCutxText->SetVisibility(false);
        m_cutSettingsCutxInvertToggle->SetVisibility(false);
        m_cutSettingsCutxInvertText->SetVisibility(false);
        m_cutSettingsCutxSlider->SetVisibility(false);
        m_cutSettingsCutyText->SetVisibility(false);
        m_cutSettingsCutyInvertToggle->SetVisibility(false);
        m_cutSettingsCutyInvertText->SetVisibility(false);
        m_cutSettingsCutySlider->SetVisibility(false);
        m_cutSettingsCutzText->SetVisibility(false);
        m_cutSettingsCutzInvertToggle->SetVisibility(false);
        m_cutSettingsCutzInvertText->SetVisibility(false);
        m_cutSettingsCutzSlider->SetVisibility(false);

        m_transferSettingsText->SetVisibility(true);
        m_transferLinearGradient->SetVisibility(true);
        for (auto& marker : m_transferMarkers)
            marker->SetVisibility(true);

        m_examinationThresholdSettingsText->SetVisibility(false);
        m_maxExaminationThresholdText->SetVisibility(false);
        m_maxExaminationThresholdValue->SetVisibility(false);
        m_minExaminationThresholdText->SetVisibility(false);
        m_minExaminationThresholdValue->SetVisibility(false);
        m_thresholdSlider->SetVisibility(false);

        toogleIsEnabledManageTabs(toggleElement);
    });
    transferTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    transferTab->Update();

    auto transferTabText = transferTab->GetTextControl();
	transferTabText->SetFontSize(TOOL_TAB_ICON_SIZE + 2);
	transferTabText->SetFont(FontTag::UI_ICON);
    transferTabText->SetSampleThreshold(-2);
    transferTabText->SetEdgeSmoothing(5);
	transferTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::TRANSFER_FUNCTION) });

    AddChildNode(transferTab);
    m_toolTabs.insert(std::pair(ToolTypes::Transfer, std::move(transferTab)));

    auto settingsTab = std::make_shared<UiToggle>("settingsCutTab", glm::uvec2(0, 0), glm::uvec2(1, 1)); 
    settingsTab->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    settingsTab->SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    settingsTab->CreateResources();

    settingsTab->SetOnEnabledFunction([this](UiToggle& toggleElement) {
        m_loadFileButton->SetVisibility(false);
        m_loadFileText->SetVisibility(false);

        m_cutSettingsText->SetVisibility(false);
        m_cutSettingsCutxText->SetVisibility(false);
        m_cutSettingsCutxInvertToggle->SetVisibility(false);
        m_cutSettingsCutxInvertText->SetVisibility(false);
        m_cutSettingsCutxSlider->SetVisibility(false);
        m_cutSettingsCutyText->SetVisibility(false);
        m_cutSettingsCutyInvertToggle->SetVisibility(false);
        m_cutSettingsCutyInvertText->SetVisibility(false);
        m_cutSettingsCutySlider->SetVisibility(false);
        m_cutSettingsCutzText->SetVisibility(false);
        m_cutSettingsCutzInvertToggle->SetVisibility(false);
        m_cutSettingsCutzInvertText->SetVisibility(false);
        m_cutSettingsCutzSlider->SetVisibility(false);

        m_transferSettingsText->SetVisibility(false);
        m_transferLinearGradient->SetVisibility(false);
        for (auto& marker : m_transferMarkers)
            marker->SetVisibility(false);

        m_examinationThresholdSettingsText->SetVisibility(true);
        m_maxExaminationThresholdText->SetVisibility(true);
        m_maxExaminationThresholdValue->SetVisibility(true);
        m_minExaminationThresholdText->SetVisibility(true);
        m_minExaminationThresholdValue->SetVisibility(true);
        m_thresholdSlider->SetVisibility(true);

        toogleIsEnabledManageTabs(toggleElement);
    });

    settingsTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    settingsTab->Update();

    auto settingsTabText = settingsTab->GetTextControl();
	settingsTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	settingsTabText->SetFont(FontTag::UI_ICON);
    settingsTabText->SetSampleThreshold(-2);
    settingsTabText->SetEdgeSmoothing(8);
	settingsTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::VOLUME_SETTINGS) });

    AddChildNode(settingsTab);
    m_toolTabs.insert(std::pair(ToolTypes::Settings, std::move(settingsTab)));

    m_toolTabsLine = std::make_shared<UiSpace>("toolTabUnderline", glm::vec2(0, 0), glm::vec2(1, 1));
    m_toolTabsLine->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_toolTabsLine->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_toolTabsLine->CreateResources();
    AddChildNode(m_toolTabsLine);

    m_cursorIndicatorText = std::make_shared<UiText>("menuCursorIndicator", glm::vec2(10.0f, 150.0f), glm::vec2(30.0f, 30.0f), L"empty");
    m_cursorIndicatorText->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_cursorIndicatorText->SetFont(FontTag::UI_ICON);
    m_cursorIndicatorText->SetFontSize(28);
    m_cursorIndicatorText->SetSampleThreshold(-2);
    m_cursorIndicatorText->SetEdgeSmoothing(5);
    m_cursorIndicatorText->SetString(std::wstring{ static_cast<wchar_t>(Icon::RIGHT_BUTTON_DELETE) });
    m_cursorIndicatorText->SetVisibility(false);
    m_cursorIndicatorText->CreateResources();
    AddChildNode(m_cursorIndicatorText);

    ResizeControls();
}

void UiMenuView::CreateLoadPanel()
{
    m_loadFileButton = std::make_shared<UiButton>("menuLoadFileButton", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_loadFileButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0));
    m_loadFileButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_loadFileButton->CreateResources();
    m_loadFileButton->SetOnClickFunction([this](UiButton& buttonElement) {
        const auto folderPathOpt = CreateFileOpenDialog(FileExtentions::FOLDER);
        if (folderPathOpt.has_value())
        {
            const auto folderPath = folderPathOpt.value();
            const auto folderName = ExtractDestinationFolderFromPath(folderPath);
            if (folderName.has_value())
            {
                m_loadFileText->SetString(std::format(L"Loading files from {} ...", folderName.value()).c_str());
                m_loadFileText->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                Application::GetEventQueue().PushEvent(std::make_shared<DataFolderSelectedEvent>(EventType::DataFolderSelected, folderPath));
            }
            else
            {
                m_loadFileText->SetString(L"Failed to extract folder");
                m_loadFileText->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            }
        }
        else
        {
            m_loadFileText->SetString(L"Failed to open folder");
            m_loadFileText->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        m_loadFileText->SetVisibility(true);
    });

    auto loadButtonTextControl = m_loadFileButton->GetTextControl();
    loadButtonTextControl->SetFontSize(MENU_TEXT_SIZE);
    loadButtonTextControl->SetString(std::wstring(L"Load file"));
    loadButtonTextControl->SetEdgeSmoothing(3.5f);
    loadButtonTextControl->SetSampleThreshold(1.0f);
    loadButtonTextControl->SetSmoothingExceptance({ L'i', L'l' });

    AddChildNode(m_loadFileButton);

    m_loadFileText = std::make_shared<UiText>("menuLoadFileText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L""));
    m_loadFileText->SetColor(glm::vec4(0.33f, 0.58f, 0.4f, 1.0f));
    m_loadFileText->SetFontSize(MENU_TEXT_SIZE);
    m_loadFileText->SetEdgeSmoothing(3.5f);
    m_loadFileText->SetSampleThreshold(1.0f);
    m_loadFileText->SetSmoothingExceptance({ L'i', L'l' });
    m_loadFileText->SetVisibility(false);
    m_loadFileText->CreateResources();

    AddChildNode(m_loadFileText);
}

void UiMenuView::CreateCutPanel()
{
    m_cutSettingsText = std::make_shared<UiText>("menuCutSettingsText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Cutting Settings"));
    m_cutSettingsText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsText->SetEdgeSmoothing(3.5f);
    m_cutSettingsText->SetSampleThreshold(1.0f);
    m_cutSettingsText->SetSmoothingExceptance({ L'i', L'l' });
    m_cutSettingsText->SetVisibility(false);
    m_cutSettingsText->CreateResources();

    AddChildNode(m_cutSettingsText);

    m_cutSettingsCutxText = std::make_shared<UiText>("menuCutSettingsCutxText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Cut X"));    
    m_cutSettingsCutxText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutxText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutxText->SetSampleThreshold(1.0f);
    m_cutSettingsCutxText->SetVisibility(false);
    m_cutSettingsCutxText->CreateResources();

    AddChildNode(m_cutSettingsCutxText);

    m_cutSettingsCutxInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutxInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutxInvertToggle->SetBackgroundColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    m_cutSettingsCutxInvertToggle->SetHoverColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
    m_cutSettingsCutxInvertToggle->SetVisibility(false);
    m_cutSettingsCutxInvertToggle->CreateResources();

    m_cutSettingsCutxInvertToggle->SetOnEnabledFunction([this](UiToggle& toggleElement) {
        auto cutSettingsCutxInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutxInvertToggleText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CLOSE) });
    });

    m_cutSettingsCutxInvertToggle->SetOnDisambledFunction([this](UiToggle& toggleElement){
        auto cutSettingsCutxInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutxInvertToggleText->SetString(std::wstring{ L" " });
    });

    auto cutSettingsCutxInvertToggleText = m_cutSettingsCutxInvertToggle->GetTextControl();
    cutSettingsCutxInvertToggleText->SetFont(FontTag::UI_ICON);
    cutSettingsCutxInvertToggleText->SetFontSize(14);
    cutSettingsCutxInvertToggleText->SetString(std::wstring{ L" " });

    AddChildNode(m_cutSettingsCutxInvertToggle);

    m_cutSettingsCutxInvertText = std::make_shared<UiText>("menuCutSettingsCutxInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Invert"));
    m_cutSettingsCutxInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutxInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutxInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutxInvertText->SetSampleThreshold(1.0f);
    m_cutSettingsCutxInvertText->SetVisibility(false);
    m_cutSettingsCutxInvertText->CreateResources();

    AddChildNode(m_cutSettingsCutxInvertText);

    m_cutSettingsCutxSlider = std::make_shared<UiSlider>("menuSettingsCutxSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutxSlider->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_cutSettingsCutxSlider->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_cutSettingsCutxSlider->SetMin(0.0f);
    m_cutSettingsCutxSlider->SetMax(100.0f);
    m_cutSettingsCutxSlider->SetValue(0.0f);
    m_cutSettingsCutxSlider->SetVisibility(false);
    m_cutSettingsCutxSlider->CreateResources();

    auto cutSettingsCutxSliderHandle = m_cutSettingsCutxSlider->GetSliderHandle();
    cutSettingsCutxSliderHandle->SetBackgroundColor(glm::vec4(0.94f, 0.164f, 0.314f, 1.0f));
    cutSettingsCutxSliderHandle->SetHoverColor(glm::vec4(1.0f, 0.3f, 0.5f, 1.0f));

    auto cutSettingsCutxSliderText = m_cutSettingsCutxSlider->GetTextControl();
    cutSettingsCutxSliderText->SetFontSize(14);
    
    AddChildNode(m_cutSettingsCutxSlider);

    m_cutSettingsCutyText = std::make_shared<UiText>("menuCutSettingsCutyText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Cut Y"));    
    m_cutSettingsCutyText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutyText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutyText->SetSampleThreshold(1.0f);
    m_cutSettingsCutyText->SetVisibility(false);
    m_cutSettingsCutyText->CreateResources();

    AddChildNode(m_cutSettingsCutyText);

    m_cutSettingsCutyInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutyInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutyInvertToggle->SetBackgroundColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    m_cutSettingsCutyInvertToggle->SetHoverColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
    m_cutSettingsCutyInvertToggle->SetVisibility(false);
    m_cutSettingsCutyInvertToggle->CreateResources();

    m_cutSettingsCutyInvertToggle->SetOnEnabledFunction([this](UiToggle& toggleElement) {
        auto cutSettingsCutyInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutyInvertToggleText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CLOSE) });
    });

    m_cutSettingsCutyInvertToggle->SetOnDisambledFunction([this](UiToggle& toggleElement){
        auto cutSettingsCutyInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutyInvertToggleText->SetString(std::wstring{ L" " });
    });

    auto cutSettingsCutyInvertToggleText = m_cutSettingsCutyInvertToggle->GetTextControl();
    cutSettingsCutyInvertToggleText->SetFont(FontTag::UI_ICON);
    cutSettingsCutyInvertToggleText->SetFontSize(14);
    cutSettingsCutyInvertToggleText->SetString(std::wstring{ L" " });

    AddChildNode(m_cutSettingsCutyInvertToggle);

    m_cutSettingsCutyInvertText = std::make_shared<UiText>("menuCutSettingsCutyInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Invert"));
    m_cutSettingsCutyInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutyInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutyInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutyInvertText->SetSampleThreshold(1.0f);
    m_cutSettingsCutyInvertText->SetVisibility(false);
    m_cutSettingsCutyInvertText->CreateResources();

    AddChildNode(m_cutSettingsCutyInvertText);

    m_cutSettingsCutySlider = std::make_shared<UiSlider>("menuSettingsCutySlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutySlider->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_cutSettingsCutySlider->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_cutSettingsCutySlider->SetMin(0.0f);
    m_cutSettingsCutySlider->SetMax(100.0f);
    m_cutSettingsCutySlider->SetValue(0.0f);
    m_cutSettingsCutySlider->SetVisibility(false);
    m_cutSettingsCutySlider->CreateResources();

    auto cutSettingsCutySliderHandle = m_cutSettingsCutySlider->GetSliderHandle();
    cutSettingsCutySliderHandle->SetBackgroundColor(glm::vec4(0.29f, 0.77f, 0.0f, 1.0f));
    cutSettingsCutySliderHandle->SetHoverColor(glm::vec4(0.5f, 0.9f, 0.1f, 1.0f));

    auto cutSettingsCutySliderText = m_cutSettingsCutySlider->GetTextControl();
    cutSettingsCutySliderText->SetFontSize(14);
    
    AddChildNode(m_cutSettingsCutySlider);

    m_cutSettingsCutzText = std::make_shared<UiText>("menuCutSettingsCutzText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Cut Z"));    
    m_cutSettingsCutzText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutzText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutzText->SetSampleThreshold(1.0f);
    m_cutSettingsCutzText->SetVisibility(false);
    m_cutSettingsCutzText->CreateResources();

    AddChildNode(m_cutSettingsCutzText);

    m_cutSettingsCutzInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutzInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutzInvertToggle->SetBackgroundColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    m_cutSettingsCutzInvertToggle->SetHoverColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
    m_cutSettingsCutzInvertToggle->SetVisibility(false);
    m_cutSettingsCutzInvertToggle->CreateResources();

    m_cutSettingsCutzInvertToggle->SetOnEnabledFunction([this](UiToggle& toggleElement) {
        auto cutSettingsCutzInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutzInvertToggleText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CLOSE) });
    });

    m_cutSettingsCutzInvertToggle->SetOnDisambledFunction([this](UiToggle& toggleElement){
        auto cutSettingsCutzInvertToggleText = toggleElement.GetTextControl();
        cutSettingsCutzInvertToggleText->SetString(std::wstring{ L" " });
    });

    auto cutSettingsCutzInvertToggleText = m_cutSettingsCutzInvertToggle->GetTextControl();
    cutSettingsCutzInvertToggleText->SetFont(FontTag::UI_ICON);
    cutSettingsCutzInvertToggleText->SetFontSize(14);
    cutSettingsCutzInvertToggleText->SetString(std::wstring{ L" " });

    AddChildNode(m_cutSettingsCutzInvertToggle);

    m_cutSettingsCutzInvertText = std::make_shared<UiText>("menuCutSettingsCutzInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), std::wstring(L"Invert"));
    m_cutSettingsCutzInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutzInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutzInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutzInvertText->SetSampleThreshold(1.0f);
    m_cutSettingsCutzInvertText->SetVisibility(false);
    m_cutSettingsCutzInvertText->CreateResources();

    AddChildNode(m_cutSettingsCutzInvertText);

    m_cutSettingsCutzSlider = std::make_shared<UiSlider>("menuSettingsCutzSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    m_cutSettingsCutzSlider->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_cutSettingsCutzSlider->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_cutSettingsCutzSlider->SetMin(0.0f);
    m_cutSettingsCutzSlider->SetMax(100.0f);
    m_cutSettingsCutzSlider->SetValue(0.0f);
    m_cutSettingsCutzSlider->SetVisibility(false);
    m_cutSettingsCutzSlider->CreateResources();

    auto cutSettingsCutzSliderHandle = m_cutSettingsCutzSlider->GetSliderHandle();
    cutSettingsCutzSliderHandle->SetBackgroundColor(glm::vec4(0.02f, 0.5f, 0.98f, 1.0f));
    cutSettingsCutzSliderHandle->SetHoverColor(glm::vec4(0.1f, 0.6f, 1.0f, 1.0f));

    auto cutSettingsCutzSliderText = m_cutSettingsCutzSlider->GetTextControl();
    cutSettingsCutzSliderText->SetFontSize(14);
    
    AddChildNode(m_cutSettingsCutzSlider);
}

void UiMenuView::CreateColorTransformPanel()
{
    m_transferSettingsText = std::make_shared<UiText>("colorTransferText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"Transfer function");
    m_transferSettingsText->SetFontSize(MENU_TEXT_SIZE);
    m_transferSettingsText->SetEdgeSmoothing(3.5f);
    m_transferSettingsText->SetSampleThreshold(1.0f);
    m_transferSettingsText->SetSmoothingExceptance({ L'i' });
    m_transferSettingsText->SetVisibility(false);
    m_transferSettingsText->CreateResources();

    AddChildNode(m_transferSettingsText);

    m_transferLinearGradient = std::make_shared<UiLinearColorGradient>("colorTransferLinearGradient", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), UiLinearColorGradient::GradientDirection::HORIZONTAL);
    m_transferLinearGradient->AddColor(GradientColorData{ 0.0f,     glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
    m_transferLinearGradient->AddColor(GradientColorData{ 0.51f,    glm::vec4(0.7f, 0.59f, 0.33f, 1.0f) });
    m_transferLinearGradient->AddColor(GradientColorData{ 0.6f,     glm::vec4(0.82f, 0.2f, 0.2f, 1.0f) });
    m_transferLinearGradient->AddColor(GradientColorData{ 0.7f,     glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
    m_transferLinearGradient->AddColor(GradientColorData{ 1.0f,     glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) });
    m_transferLinearGradient->CreateResources();

    AddChildNode(m_transferLinearGradient);
}

void UiMenuView::CreateMarkersForTransferFunction()
{
    while (m_transferMarkers.size() > 0)
    {
        auto marker = m_transferMarkers.back();
        m_transferMarkers.pop_back();
        RemoveChildNode(std::move(marker));
    }

    auto areaSizeChangedEvent = std::make_shared<RenderAreaSizeChanged>(EventType::RenderAreaSizeChanged);
    const auto& colors = m_transferLinearGradient->GetGradientColors();
    const auto gradientPosition = m_transferLinearGradient->GetPosition();
    const auto gradientSize = m_transferLinearGradient->GetSize();
    const auto markerName = m_transferLinearGradient->GetName();
    const auto visible = m_transferLinearGradient->IsVisible();
    unsigned markerIndex = 0u;

    for (const auto& color : colors)
    {
        const auto markerPosition = gradientPosition + glm::vec2(gradientSize.x * color.position, gradientSize.y) - glm::vec2(MARKER_SIZE / 2.0f, -MARKER_SIZE * 0.75);
        auto marker = std::make_shared<UiMarker>(std::format("{}_{}", markerName, markerIndex), markerPosition, glm::vec2(MARKER_SIZE, MARKER_SIZE * 1.5f));
        marker->SetDragBounderies(gradientPosition.x - MARKER_SIZE / 2.0f, gradientPosition.x - MARKER_SIZE / 2.0f + gradientSize.x);
        marker->SetMarkerColor(color.color);
        marker->SetVisibility(visible);
        marker->CreateResources();
        AddChildNode(marker);
        marker->HandleEvent(areaSizeChangedEvent.get());
        m_transferMarkers.push_back(std::move(marker));
        markerIndex++;
    }
}

void UiMenuView::CreateSettingsPanel()
{
    m_examinationThresholdSettingsText = std::make_shared<UiText>("examinationThresholdSettingsText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"Examination threshold");
    m_examinationThresholdSettingsText->SetFontSize(MENU_TEXT_SIZE);
    m_examinationThresholdSettingsText->SetEdgeSmoothing(3.5f);
    m_examinationThresholdSettingsText->SetSampleThreshold(1.0f);
    m_examinationThresholdSettingsText->SetSmoothingExceptance({ L'i', L'l'});
    m_examinationThresholdSettingsText->SetVisibility(false);
    m_examinationThresholdSettingsText->CreateResources();

    AddChildNode(m_examinationThresholdSettingsText);

    m_minExaminationThresholdText = std::make_shared<UiText>("minExaminationThresholdText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"Min");
    m_minExaminationThresholdText->SetFontSize(MENU_TEXT_SIZE);
    m_minExaminationThresholdText->SetEdgeSmoothing(3.5f);
    m_minExaminationThresholdText->SetSampleThreshold(1.0f);
    m_minExaminationThresholdText->SetSmoothingExceptance({ L'i' });
    m_minExaminationThresholdText->SetVisibility(true);
    m_minExaminationThresholdText->CreateResources();

    AddChildNode(m_minExaminationThresholdText);

    m_minExaminationThresholdValue = std::make_shared<UiText>("minExaminationThresholdValue", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"0");
    m_minExaminationThresholdValue->SetFontSize(MENU_TEXT_SIZE);
    m_minExaminationThresholdValue->SetEdgeSmoothing(3.5f);
    m_minExaminationThresholdValue->SetSampleThreshold(1.0f);
    m_minExaminationThresholdValue->SetSmoothingExceptance({ L'-' });
    m_minExaminationThresholdValue->SetVisibility(true);
    m_minExaminationThresholdValue->CreateResources();

    AddChildNode(m_minExaminationThresholdValue);

    m_maxExaminationThresholdText = std::make_shared<UiText>("maxExaminationThresholdText", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"Max");
    m_maxExaminationThresholdText->SetFontSize(MENU_TEXT_SIZE);
    m_maxExaminationThresholdText->SetEdgeSmoothing(3.5f);
    m_maxExaminationThresholdText->SetSampleThreshold(1.0f);
    m_maxExaminationThresholdText->SetVisibility(true);
    m_maxExaminationThresholdText->CreateResources();

    AddChildNode(m_maxExaminationThresholdText);

    m_maxExaminationThresholdValue = std::make_shared<UiText>("maxExaminationThresholdValue", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), L"1");
    m_maxExaminationThresholdValue->SetFontSize(MENU_TEXT_SIZE);
    m_maxExaminationThresholdValue->SetEdgeSmoothing(3.5f);
    m_maxExaminationThresholdValue->SetSampleThreshold(1.0f);
    m_maxExaminationThresholdValue->SetSmoothingExceptance({ L'-' });
    m_maxExaminationThresholdValue->SetVisibility(true);
    m_maxExaminationThresholdValue->CreateResources();

    AddChildNode(m_maxExaminationThresholdValue);

    m_thresholdSlider = std::make_shared<UiTwoSideSlider>("volumeThresholdSlider", glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 15.0f));
    m_thresholdSlider->CreateResources();
    auto thresholdMainSlider = m_thresholdSlider->GetMainSliderHandle();
    thresholdMainSlider->SetBackgroundColor(glm::vec4(0.02f, 0.5f, 0.98f, 1.0f));

    AddChildNode(m_thresholdSlider);
}

void UiMenuView::ResizeControls()
{
    CreateFrameBuffer();

    const auto renderAreaSize = GetSize();
    const auto tabWidth = renderAreaSize.x / TOOL_BAR_SIZE;
    const auto tabHeight = 40;
    auto uiCursor = glm::vec2(0.0f, 0.0f);

    auto loadTab = m_toolTabs[ToolTypes::Load];
    loadTab->SetPosition(uiCursor);
    loadTab->SetSize(glm::vec2(tabWidth, tabHeight));
    uiCursor += glm::vec2(tabWidth, 0.0f);

    auto cutTab = m_toolTabs[ToolTypes::Cut];
    cutTab->SetPosition(uiCursor);
    cutTab->SetSize(glm::vec2(tabWidth, tabHeight));
    uiCursor += glm::vec2(tabWidth, 0.0f);

    auto transferTab = m_toolTabs[ToolTypes::Transfer];
    transferTab->SetPosition(uiCursor);
    transferTab->SetSize(glm::vec2(tabWidth, tabHeight));
    uiCursor += glm::vec2(tabWidth, 0.0f);

    auto settingsTab = m_toolTabs[ToolTypes::Settings];
    settingsTab->SetPosition(uiCursor);
    settingsTab->SetSize(glm::vec2(tabWidth, tabHeight));
    uiCursor = glm::vec2(0.0f, tabHeight);

    m_toolTabsLine->SetPosition(uiCursor);
    m_toolTabsLine->SetSize(glm::vec2(renderAreaSize.x, 1.0f));

    // Load panel
    uiCursor = glm::vec2((renderAreaSize.x * 0.5f) - (renderAreaSize.x * 0.4f), tabHeight * 1.5f);
    m_loadFileButton->SetPosition(uiCursor);
    m_loadFileButton->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    uiCursor += glm::vec2(0.5f, tabHeight);
    m_loadFileText->SetPosition(uiCursor);
    m_loadFileText->SetSize(m_loadFileButton->GetSize());

    // Cut panel 
    uiCursor = glm::vec2((renderAreaSize.x * 0.5f) - (renderAreaSize.x * 0.4f), tabHeight * 1.5f);
    m_cutSettingsText->SetPosition(uiCursor);
    m_cutSettingsText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    uiCursor += glm::vec2(0.5f, tabHeight * 0.75f);
    m_cutSettingsCutxText->SetPosition(uiCursor);
    m_cutSettingsCutxText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutxInvertToggle->SetPosition(uiCursor);
    m_cutSettingsCutxInvertToggle->SetSize(glm::vec2(20.0f, 20.0f));

    uiCursor += glm::vec2(m_cutSettingsCutxInvertToggle->GetSize().x + 9.5f, 0.0f);
    m_cutSettingsCutxInvertText->SetPosition(uiCursor);
    m_cutSettingsCutxInvertText->SetSize(glm::vec2(renderAreaSize.x / 2.0f, m_cutSettingsCutxInvertToggle->GetSize().y));

    uiCursor -= glm::vec2(m_cutSettingsCutxInvertToggle->GetSize().x + 9.5f, 0.0f);
    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutxSlider->SetPosition(uiCursor);
    m_cutSettingsCutxSlider->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 20));

    uiCursor += glm::vec2(0.5f, tabHeight * 0.75f);
    m_cutSettingsCutyText->SetPosition(uiCursor);
    m_cutSettingsCutyText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutyInvertToggle->SetPosition(uiCursor);
    m_cutSettingsCutyInvertToggle->SetSize(glm::vec2(20.0f, 20.0f));

    uiCursor += glm::vec2(m_cutSettingsCutyInvertToggle->GetSize().x + 9.0f, 0.0f);
    m_cutSettingsCutyInvertText->SetPosition(uiCursor);
    m_cutSettingsCutyInvertText->SetSize(glm::vec2(renderAreaSize.x / 2.0f, m_cutSettingsCutyInvertToggle->GetSize().y));

    uiCursor -= glm::vec2(m_cutSettingsCutxInvertToggle->GetSize().x + 9.0f, 0.0f);
    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutySlider->SetPosition(uiCursor);
    m_cutSettingsCutySlider->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 20));


    uiCursor += glm::vec2(0.5f, tabHeight * 0.75f);
    m_cutSettingsCutzText->SetPosition(uiCursor);
    m_cutSettingsCutzText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutzInvertToggle->SetPosition(uiCursor);
    m_cutSettingsCutzInvertToggle->SetSize(glm::vec2(20.0f, 20.0f));

    uiCursor += glm::vec2(m_cutSettingsCutzInvertToggle->GetSize().x + 9.5f, 0.0f);
    m_cutSettingsCutzInvertText->SetPosition(uiCursor);
    m_cutSettingsCutzInvertText->SetSize(glm::vec2(renderAreaSize.x / 2.0f, m_cutSettingsCutzInvertToggle->GetSize().y));

    uiCursor -= glm::vec2(m_cutSettingsCutzInvertToggle->GetSize().x + 9.0f, 0.0f);
    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_cutSettingsCutzSlider->SetPosition(uiCursor);
    m_cutSettingsCutzSlider->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 20));

    // Color transfer panel
    uiCursor = glm::vec2((renderAreaSize.x * 0.5f) - (renderAreaSize.x * 0.4f), tabHeight * 1.5f);

    m_transferSettingsText->SetPosition(uiCursor);
    m_transferSettingsText->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 30.0f));
    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);

    m_transferLinearGradient->SetPosition(uiCursor);
    m_transferLinearGradient->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 30.0f));
    CreateMarkersForTransferFunction();

    // Settings panel
    uiCursor = glm::vec2((renderAreaSize.x * 0.5f) - (renderAreaSize.x * 0.4f), tabHeight * 1.5f);

    m_examinationThresholdSettingsText->SetPosition(uiCursor);
    m_examinationThresholdSettingsText->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 30.0f));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.75f);
    m_minExaminationThresholdText->SetPosition(uiCursor);
    m_minExaminationThresholdText->SetSize(glm::vec2(renderAreaSize.x * 0.3f, 30.0f));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.5f);

    m_minExaminationThresholdValue->SetPosition(uiCursor);
    m_minExaminationThresholdValue->SetSize(glm::vec2(renderAreaSize.x * 0.3f, 30.0f));

    uiCursor += glm::vec2(renderAreaSize.x - (renderAreaSize.x * 0.3f), -tabHeight * 0.5f);
    m_maxExaminationThresholdText->SetPosition(uiCursor);
    m_maxExaminationThresholdText->SetSize(glm::vec2(renderAreaSize.x * 0.3f, 30.0f));

    uiCursor += glm::vec2(0.0f, tabHeight * 0.5f);
    m_maxExaminationThresholdValue->SetPosition(uiCursor);
    m_maxExaminationThresholdValue->SetSize(glm::vec2(renderAreaSize.x * 0.3f, 30.0f));

    uiCursor += glm::vec2(-(renderAreaSize.x - (renderAreaSize.x * 0.3f)), tabHeight * 0.5f);
    m_thresholdSlider->SetPosition(uiCursor);
    m_thresholdSlider->SetSize(glm::vec2(glm::vec2(renderAreaSize.x * 0.8f, 15.0f)));
}
