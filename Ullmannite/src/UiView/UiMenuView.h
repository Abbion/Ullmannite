#pragma once
#include "UiElement/UiRenderArea.h"
#include "UiElement/Controls/AllControls.h"
#include "DataStructures/CuttingSettings.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <array>
#include <optional>

namespace Ull
{
    struct FileInfo;

    class UiMenuView : public UiRenderArea
    {
    public:
        UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size);
        virtual ~UiMenuView() { }

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

    private:
        enum class ToolTypes {
            Load,
            Cut,
            Transfer,
            Settings
        };

    private:
		void CreateControls();
		void ResizeControls();

		void Init();
        void RenderUI();
        void RenderLoadTab();
        void RenderCutSettings();
        void RenderTransferFunctionSettings();
        void RenderDataSettings();

        std::optional<std::wstring> m_filePath;
        bool m_firstDataLoaded{ false };
        bool m_newDataLoaded{ false };
        bool m_renderTransferEditor{ false };

        glm::ivec2 m_cubeMarchTresholds{ 0, 0 };
        CuttingSettings m_cuttingSettings;
        std::unordered_map<ToolTypes, std::shared_ptr<UiToggle>> m_toolTabs;
        std::shared_ptr<UiSpace> m_toolTabsLine;
        
        //Load controls
        std::shared_ptr<UiButton> m_loadFileButton;
        std::shared_ptr<UiText> m_loadFileText;

        //Cut controls
        std::shared_ptr<UiText>     m_cutSettingsText;
        std::shared_ptr<UiText>     m_cutSettingsCutxText;
        std::shared_ptr<UiToggle>   m_cutSettingsCutxInvertToggle;
        std::shared_ptr<UiText>     m_cutSettingsCutxInvertText;
        std::shared_ptr<UiSlider>   m_cutSettingsCutxSlider;


        std::shared_ptr<UiText> m_cutSettingsCutyText;
        std::shared_ptr<UiText> m_cutSettingsCutyInvertText;
        std::shared_ptr<UiText> m_cutSettingsCutzText;
        std::shared_ptr<UiText> m_cutSettingsCutzInvertText;
    };
}