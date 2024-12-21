#include "Ullpch.h"
#include "UiSpace.h"

namespace Ull
{
    UiSpace::UiSpace(const std::string name, const glm::uvec2 position, const glm::uvec2 size) :
        UiBasicControl(name, position, size, UiControlType::UiSpace)
    {
    }
}