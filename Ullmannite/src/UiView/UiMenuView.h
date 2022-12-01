#pragma once
#include "UiElement/UiArea.h"
#include "DataStructures/CuttingSettings.h"
#include "UiElement/UiGradientEditor.h"
#include "Rendering/Objects/TransferFunctionRenderer.h"
#include <array>
#include <optional>

namespace Ull
{
    struct FileInfo;

    class UiMenuView : public UiArea
    {
    public:
        UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

         TransferFunctionRenderer* GetTransferFunctionRenderer() { return &m_transferFunction; }

    private:
		void Init();
        void RenderUI();
        void RenderLoadTab();
        void RenderCutSettings();
        void RenderTransferFunctionSettings();
        void RenderDataSettings();

        std::optional<std::wstring > m_filePath;
        bool m_firstDataLoaded{ false };
        bool m_newDataLoaded{ false };
        bool m_renderTransferEditor{ false };

        glm::ivec2 m_cubeMarchTresholds{ 0, 0 };
        CuttingSettings m_cuttingSettings;
        
        UiGradientEditor m_gradientEditor;
        TransferFunctionRenderer m_transferFunction;
    };
}