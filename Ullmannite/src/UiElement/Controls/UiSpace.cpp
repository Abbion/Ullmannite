#include "Ullpch.h"
#include "UiSpace.h"

namespace Ull
{
    UiSpace::UiSpace(const std::string name, const glm::vec2 position, const glm::vec2 size) :
        UiBasicControl(name, position, size, UiControlType::UiSpace)
    {
    }
}