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
    // Load panel
    m_loadFileButton = std::make_shared<UiButton>("menuLoadFileButton", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
    m_loadFileButton->SetBackgroundColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0));
    m_loadFileButton->SetHoverColor(glm::vec4(0.33f, 0.33f, 0.33f, 1.0f));
    m_loadFileButton->CreateResources();
    m_loadFileButton->SetOnClickFunction([this](UiButton& buttonElement) {
        m_loadFileText->SetString(std::wstring(L"File loaded: Test"));
        m_loadFileText->SetVisibility(true);
    });

    AddChildNode(m_loadFileButton);

    auto& loadButtonTextControl = m_loadFileButton->GetTextControl();
    loadButtonTextControl->SetFontSize(MENU_TEXT_SIZE);
    loadButtonTextControl->SetString(std::wstring(L"Load file"));
    loadButtonTextControl->SetEdgeSmoothing(3.5f);
    loadButtonTextControl->SetSampleThreshold(1.0f);
    loadButtonTextControl->SetSmoothingExceptance({ L'i', L'l' });

    m_loadFileText = std::make_shared<UiText>("menuLoadFileText", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), std::wstring(L"File loaded:"));
    m_loadFileText->SetColor(glm::vec4(0.33f, 0.58f, 0.4f, 1.0f));
    m_loadFileText->SetFontSize(MENU_TEXT_SIZE);
    m_loadFileText->SetEdgeSmoothing(3.5f);
    m_loadFileText->SetSampleThreshold(1.5f);
    m_loadFileText->SetSmoothingExceptance({ L'i', L'l' });
    m_loadFileText->SetVisibility(false);
    m_loadFileText->CreateResources();

    AddChildNode(m_loadFileText);

    // Cut panel
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
    m_cutSettingsCutxSlider->SetBackgroundColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    m_cutSettingsCutxSlider->SetFunctionality(UiControlFunctionality::Hover, State::Disable);
    m_cutSettingsCutxSlider->SetMin(0.0f);
    m_cutSettingsCutxSlider->SetMax(100.0f);
    m_cutSettingsCutxSlider->SetValue(0.0f);
    m_cutSettingsCutxSlider->CreateResources();

    auto cutSettingsCutxSliderHandle = m_cutSettingsCutxSlider->GetSliderHandle();
    cutSettingsCutxSliderHandle->SetBackgroundColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    
    AddChildNode(m_cutSettingsCutxSlider);

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

    transferTab->SetOnEnabledFunction(toogleIsEnabledManageTabs);
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

void UiMenuView::ResizeControls()
{
    const auto renderAreaSize = GetSize();
    const auto tabWidth = renderAreaSize.x / TOOL_BAR_SIZE;
    const auto tabHeight = 40;

    auto loadTab = m_toolTabs[ToolTypes::Load];
    loadTab->SetSize(glm::vec2(tabWidth, tabHeight));

    auto cutTab = m_toolTabs[ToolTypes::Cut];
    cutTab->SetPosition(glm::vec2(tabWidth, 0.0f));
    cutTab->SetSize(glm::vec2(tabWidth, tabHeight));

    auto transferTab = m_toolTabs[ToolTypes::Transfer];
    transferTab->SetPosition(glm::vec2(tabWidth * 2.0f, 0.0f));
    transferTab->SetSize(glm::vec2(tabWidth, tabHeight));

    auto settingsTab = m_toolTabs[ToolTypes::Settings];
    settingsTab->SetPosition(glm::vec2(tabWidth * 3.0f, 0.0f));
    settingsTab->SetSize(glm::vec2(tabWidth, tabHeight));

    m_toolTabsLine->SetPosition(glm::vec2(0.0f, tabHeight));
    m_toolTabsLine->SetSize(glm::vec2(renderAreaSize.x, 1.0f));

    // Load panel
    m_loadFileButton->SetPosition(glm::vec2((renderAreaSize.x / 2.0f) - (renderAreaSize.x * 0.4f), tabHeight * 1.25f));
    m_loadFileButton->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    m_loadFileText->SetPosition(m_loadFileButton->GetPosition() + glm::vec2(0.5f, tabHeight));
    m_loadFileText->SetSize(m_loadFileButton->GetSize());

    // Cut panel
    m_cutSettingsText->SetPosition(glm::vec2((renderAreaSize.x / 2.0f) - (renderAreaSize.x * 0.4f), tabHeight * 1.25f));
    m_cutSettingsText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    m_cutSettingsCutxText->SetPosition(glm::vec2((renderAreaSize.x / 2.0f) - (renderAreaSize.x * 0.4f), tabHeight * 2.0f));
    m_cutSettingsCutxText->SetSize(glm::vec2(0.8f * renderAreaSize.x, 30));

    m_cutSettingsCutxInvertToggle->SetPosition(m_cutSettingsCutxText->GetPosition() + glm::vec2(0.0f, tabHeight / 1.5f));
    m_cutSettingsCutxInvertToggle->SetSize(glm::vec2(25.0f, 25.0f));

    m_cutSettingsCutxInvertText->SetPosition(m_cutSettingsCutxInvertToggle->GetPosition() +
                                             glm::vec2(m_cutSettingsCutxInvertToggle->GetSize().x + 10.f, 0.0f));
    m_cutSettingsCutxInvertText->SetSize(glm::vec2(renderAreaSize.x / 2.0f, m_cutSettingsCutxInvertToggle->GetSize().y));

    auto uiCursor = m_cutSettingsCutxInvertToggle->GetPosition() + glm::vec2(0.0f, tabHeight / 1.5f);
    m_cutSettingsCutxSlider->SetPosition(uiCursor);
    m_cutSettingsCutxSlider->SetSize(glm::vec2(renderAreaSize.x * 0.8f, 30));
}
