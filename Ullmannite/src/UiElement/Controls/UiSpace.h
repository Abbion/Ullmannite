#pragma once

#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiSpace : public UiBasicControl
    {
    public:
        UiSpace(const std::string name, const glm::uvec2 position, const glm::uvec2 size);

    };
}