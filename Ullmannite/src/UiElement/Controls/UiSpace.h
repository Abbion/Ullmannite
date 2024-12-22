#pragma once

#include "UiElement/Controls/UiBasicControl.h"

namespace Ull
{
    class UiSpace : public UiBasicControl
    {
    public:
        UiSpace(const std::string name, const glm::vec2 position, const glm::vec2 size);

    };
}