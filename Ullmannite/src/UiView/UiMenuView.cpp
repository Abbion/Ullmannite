#include "Ullpch.h"
#include "UiMenuView.h"
#include "Core/PlatformDependantFreeFunctions.h"
#include "Logger/Logger.h"
#include "Event/EventAggregator.h"
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
}

UiMenuView::UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size) :
    UiRenderArea(name, position, size, false)
{
    SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    
    CreateControls();

    m_cubeMarchTresholds.x = 0;
    m_cubeMarchTresholds.y = maxUint16;

    m_cuttingSettings.cuttingPositions = glm::vec3(100.0f, 100.0f, 100.0f);
    m_cuttingSettings.invertedAxis = { false, false, false };
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
    }
    break;

    break;
    }

	UiRenderArea::HandleEvent(event);
}

void UiMenuView::Update()
{
    //m_areaUpdated = true;
	UiRenderArea::Update();
}

void UiMenuView::Render()
{
    if(m_areaUpdated)
    {
        m_frameBuffer->Bind();
        RenderBackground();
        m_frameBuffer->Unbind();

        m_areaUpdated = false;
    }

	RenderUI();
}

void UiMenuView::RenderUI()
{
	UiRenderArea::Render();
}

void UiMenuView::CreateControls()
{
    CreateLoadPanel();
    CreateCutPanel();

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
        
        toogleIsEnabledManageTabs(toggleElement);
       });

    loadTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    loadTab->SetEnabled(true);
    loadTab->Update();

    auto& loadTabText = loadTab->GetTextControl();
	loadTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	loadTabText->SetFont(FontTag::UI_ICON);
    loadTabText->SetSampleThreshold(-2);
    loadTabText->SetEdgeSmoothing(5);
	loadTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::LOAD_FILE) });

    AddChildNode(loadTab);
    m_toolTabs.insert(std::pair(ToolTypes::Load, std::move(loadTab)));

    auto cutTab = std::make_shared<UiToggle>("menuCutTab", glm::uvec2(0, 0), glm::uvec2(0, 0)); 
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

        toogleIsEnabledManageTabs(toggleElement);
    });
    cutTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    cutTab->Update();

    auto& cutTabText = cutTab->GetTextControl();
	cutTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	cutTabText->SetFont(FontTag::UI_ICON);
    cutTabText->SetSampleThreshold(-2);
    cutTabText->SetEdgeSmoothing(5);
	cutTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::CUTING_SETTINGS) });

    AddChildNode(cutTab);
    m_toolTabs.insert(std::pair(ToolTypes::Cut, std::move(cutTab)));

    auto transferTab = std::make_shared<UiToggle>("transferCutTab", glm::uvec2(0, 0), glm::uvec2(0, 0)); 
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

        toogleIsEnabledManageTabs(toggleElement);
    });
    transferTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    transferTab->Update();

    auto& transferTabText = transferTab->GetTextControl();
	transferTabText->SetFontSize(TOOL_TAB_ICON_SIZE + 2);
	transferTabText->SetFont(FontTag::UI_ICON);
    transferTabText->SetSampleThreshold(-2);
    transferTabText->SetEdgeSmoothing(5);
	transferTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::TRANSFER_FUNCTION) });

    AddChildNode(transferTab);
    m_toolTabs.insert(std::pair(ToolTypes::Transfer, std::move(transferTab)));

    auto settingsTab = std::make_shared<UiToggle>("settingsCutTab", glm::uvec2(0, 0), glm::uvec2(0, 0)); 
    settingsTab->SetBackgroundColor(glm::vec4(0.149f, 0.149f, 0.149f, 1.0f));
    settingsTab->SetHoverColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    settingsTab->CreateResources();

    settingsTab->SetOnEnabledFunction(toogleIsEnabledManageTabs);
    settingsTab->SetOnDisambledFunction(toogleIsDisabledManageTabs);

    settingsTab->Update();

    auto& settingsTabText = settingsTab->GetTextControl();
	settingsTabText->SetFontSize(TOOL_TAB_ICON_SIZE);
	settingsTabText->SetFont(FontTag::UI_ICON);
    settingsTabText->SetSampleThreshold(-2);
    settingsTabText->SetEdgeSmoothing(8);
	settingsTabText->SetString(std::wstring{ static_cast<wchar_t>(Icon::VOLUME_SETTINGS) });

    AddChildNode(settingsTab);
    m_toolTabs.insert(std::pair(ToolTypes::Settings, std::move(settingsTab)));

    m_toolTabsLine = std::make_shared<UiSpace>("toolTabUnderline", glm::vec2(0, 0), glm::vec2(0, 0));
    m_toolTabsLine->SetBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_toolTabsLine->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_toolTabsLine->CreateResources();
    AddChildNode(m_toolTabsLine);

    ResizeControls();
}

void UiMenuView::CreateLoadPanel()
{
    m_loadFileButton = std::make_shared<UiButton>("menuLoadFileButton", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    m_loadFileButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0));
    m_loadFileButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_loadFileButton->CreateResources();
    m_loadFileButton->SetOnClickFunction([this](UiButton& buttonElement) {
        m_loadFileText->SetString(std::wstring(L"File loaded: Test"));
        m_loadFileText->SetVisibility(true);
    });

    auto& loadButtonTextControl = m_loadFileButton->GetTextControl();
    loadButtonTextControl->SetFontSize(MENU_TEXT_SIZE);
    loadButtonTextControl->SetString(std::wstring(L"Load file"));
    loadButtonTextControl->SetEdgeSmoothing(3.5f);
    loadButtonTextControl->SetSampleThreshold(1.0f);
    loadButtonTextControl->SetSmoothingExceptance({ L'i', L'l' });

    AddChildNode(m_loadFileButton);

    m_loadFileText = std::make_shared<UiText>("menuLoadFileText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"File loaded:"));
    m_loadFileText->SetColor(glm::vec4(0.33f, 0.58f, 0.4f, 1.0f));
    m_loadFileText->SetFontSize(MENU_TEXT_SIZE);
    m_loadFileText->SetEdgeSmoothing(3.5f);
    m_loadFileText->SetSampleThreshold(1.5f);
    m_loadFileText->SetSmoothingExceptance({ L'i', L'l' });
    m_loadFileText->SetVisibility(false);
    m_loadFileText->CreateResources();

    AddChildNode(m_loadFileText);
}

void UiMenuView::CreateCutPanel()
{
    m_cutSettingsText = std::make_shared<UiText>("menuCutSettingsText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Cutting Settings"));
    m_cutSettingsText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsText->SetEdgeSmoothing(3.5f);
    m_cutSettingsText->SetSampleThreshold(1.5f);
    m_cutSettingsText->SetSmoothingExceptance({ L'i', L'l' });
    m_cutSettingsText->SetVisibility(false);
    m_cutSettingsText->CreateResources();

    AddChildNode(m_cutSettingsText);

    m_cutSettingsCutxText = std::make_shared<UiText>("menuCutSettingsCutxText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Cut X"));    
    m_cutSettingsCutxText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutxText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutxText->SetSampleThreshold(1.5f);
    m_cutSettingsCutxText->SetVisibility(false);
    m_cutSettingsCutxText->CreateResources();

    AddChildNode(m_cutSettingsCutxText);

    m_cutSettingsCutxInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutxInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
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

    m_cutSettingsCutxInvertText = std::make_shared<UiText>("menuCutSettingsCutxInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Invert"));
    m_cutSettingsCutxInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutxInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutxInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutxInvertText->SetSampleThreshold(1.5f);
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

    m_cutSettingsCutyText = std::make_shared<UiText>("menuCutSettingsCutyText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Cut Y"));    
    m_cutSettingsCutyText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutyText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutyText->SetSampleThreshold(1.5f);
    m_cutSettingsCutyText->SetVisibility(false);
    m_cutSettingsCutyText->CreateResources();

    AddChildNode(m_cutSettingsCutyText);

    m_cutSettingsCutyInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutyInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
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

    m_cutSettingsCutyInvertText = std::make_shared<UiText>("menuCutSettingsCutyInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Invert"));
    m_cutSettingsCutyInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutyInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutyInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutyInvertText->SetSampleThreshold(1.5f);
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

    m_cutSettingsCutzText = std::make_shared<UiText>("menuCutSettingsCutzText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Cut Z"));    
    m_cutSettingsCutzText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutzText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutzText->SetSampleThreshold(1.5f);
    m_cutSettingsCutzText->SetVisibility(false);
    m_cutSettingsCutzText->CreateResources();

    AddChildNode(m_cutSettingsCutzText);

    m_cutSettingsCutzInvertToggle = std::make_shared<UiToggle>("menuCutSettingsCutzInvertToggle", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
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

    m_cutSettingsCutzInvertText = std::make_shared<UiText>("menuCutSettingsCutzInvertText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"Invert"));
    m_cutSettingsCutzInvertText->SetAlignment(UiText::HorizontalAlignment::LEFT, UiText::VerticalAlignment::CENTER);
    m_cutSettingsCutzInvertText->SetFontSize(MENU_TEXT_SIZE);
    m_cutSettingsCutzInvertText->SetEdgeSmoothing(3.5f);
    m_cutSettingsCutzInvertText->SetSampleThreshold(1.5f);
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

void UiMenuView::ResizeControls()
{
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
}
