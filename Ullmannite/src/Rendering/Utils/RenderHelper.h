#pragma once
#include <glm/glm.hpp>

namespace Ull
{
    glm::vec3 RgbToHsl(const glm::vec3 rgbValue);
    glm::vec3 HslToRgb(const glm::vec3 hslValue);

    glm::vec3 RgbToHsv(const glm::vec3 rgbValue);
    glm::vec3 HsvToRgb(const glm::vec3 hsvValue);
}