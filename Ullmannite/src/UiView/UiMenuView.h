#pragma once
#include "UiElement/UiArea.h"
#include <optional>

namespace Ull
{
    class UiMenuView : public UiArea
    {
    public:
        UiMenuView(std::string name, glm::uvec2 position, glm::uvec2 size);

		void HandleEvent(Event* event) override;
		void Update() override;
		void Render() override;

    private:
		void Init();
        void RenderUI();
        void RenderLoadTab();
        void RenderMirrorSettings();
        void RenderTransferFunctionSettings();
        void RenderDataSettings();

        std::optional<std::wstring> m_filePath;
        bool m_firstDataLoaded{ false };
        bool m_newDataLoaded{ false };

        glm::ivec2 m_cubeMarchTresholds{ 0, 0 };
    };
}