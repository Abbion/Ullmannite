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
		void ResizeControls();

    private:
        enum class ToolTypes {
            Load,
            Cut,
            Transfer,
            Settings
        };

    private:
		void CreateControls();
        void CreateLoadPanel();
        void CreateCutPanel();
        void CreateColorTransformPanel();
        void CreateMarkersForTransferFunction();
        void CreateSettingsPanel();

		void Init();
        void RenderUI();

        std::optional<std::wstring> m_filePath;
        bool m_firstDataLoaded{ false };
        bool m_newDataLoaded{ false };
        bool m_renderTransferEditor{ false };

        glm::ivec2 m_cubeMarchTresholds{ 0, 0 };
        CuttingSettings m_cuttingSettings;
        std::unordered_map<ToolTypes, std::shared_ptr<UiToggle>> m_toolTabs;
        std::shared_ptr<UiSpace> m_toolTabsLine;
        
        // Load controls
        std::shared_ptr<UiButton>       m_loadFileButton;
        std::shared_ptr<UiText>         m_loadFileText;

        // Cut controls
        std::shared_ptr<UiText>         m_cutSettingsText;

        std::shared_ptr<UiText>         m_cutSettingsCutxText;
        std::shared_ptr<UiToggle>       m_cutSettingsCutxInvertToggle;
        std::shared_ptr<UiText>         m_cutSettingsCutxInvertText;
        std::shared_ptr<UiSlider>       m_cutSettingsCutxSlider;

        std::shared_ptr<UiText>         m_cutSettingsCutyText;
        std::shared_ptr<UiToggle>       m_cutSettingsCutyInvertToggle;
        std::shared_ptr<UiText>         m_cutSettingsCutyInvertText;
        std::shared_ptr<UiSlider>       m_cutSettingsCutySlider;

        std::shared_ptr<UiText>         m_cutSettingsCutzText;
        std::shared_ptr<UiToggle>       m_cutSettingsCutzInvertToggle;
        std::shared_ptr<UiText>         m_cutSettingsCutzInvertText;
        std::shared_ptr<UiSlider>       m_cutSettingsCutzSlider;

        // Transfer Controls
        std::shared_ptr<UiText>                 m_transferSettingsText;
        std::shared_ptr<UiLinearColorGradient>  m_transferLinearGradient;
        std::vector<std::shared_ptr<UiMarker>>  m_transferMarkers;
        std::shared_ptr<UiText>                 m_cursorIndicatorText;

        // Settings control
        std::shared_ptr<UiText>             m_examinationThresholdSettingsText;
        std::shared_ptr<UiText>             m_maxExaminationThresholdText;
        std::shared_ptr<UiText>             m_maxExaminationThresholdValue;
        std::shared_ptr<UiText>             m_minExaminationThresholdText;
        std::shared_ptr<UiText>             m_minExaminationThresholdValue;
        std::shared_ptr<UiTwoSideSlider>    m_thresholdSlider;
    };
}
