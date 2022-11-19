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

        std::optional<std::wstring> m_filePath;
        bool m_firstDataLoaded{ false };
    };
}