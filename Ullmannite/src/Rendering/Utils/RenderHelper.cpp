#include "Ullpch.h"
#include "RenderHelper.h"
#include <cmath>

glm::vec3 Ull::RgbToHsl(const glm::vec3 rgbValue)
{
    const auto c_max = max(max(rgbValue.r, rgbValue.g), rgbValue.b);
    const auto c_min = min(min(rgbValue.r, rgbValue.g), rgbValue.b);
    const auto delta = c_max - c_min;

    const auto L = (c_max + c_min) / 2.0f;

    const auto H = [&]() -> float {
        if (IsZero(delta)) {
            return 0.0f;
        }
        else if (c_max == rgbValue.r) {
            const auto segment = 60.0f * ((rgbValue.g - rgbValue.b) / delta);
            if (segment < 0.0f)
                return 360.0f + segment;
            return segment;
        }
        else if (c_max == rgbValue.g) {
            return 60.0f * (((rgbValue.b - rgbValue.r) / delta) + 2.0f);
        }   
        else if (c_max == rgbValue.b) {
            return 60 * (((rgbValue.r - rgbValue.g) / delta) + 4.0f);
        }
        
        return 0.0f;
    }();

    const auto S = [&]() -> float {
        if (IsZero(delta)){
            return 0.0f;
        }
        
        return delta / (1.0f - abs(2.0f * L - 1));
    }();

    return glm::vec3(H, S, L);
}

glm::vec3 Ull::HslToRgb(const glm::vec3 hslValue)
{
    const auto C = (1.0f - abs(2.0f * hslValue.z - 1.0f)) * hslValue.y;
    const auto H = hslValue.x / 60.f;
    const auto X = C * (1.0f - abs(fmod(H, 2.0f) - 1.0f));

    const auto intermediateRgb = [&](){
        if (H >= 0.0f && H < 1.0f)
            return glm::vec3(C, X, 0.0f);
        if (H >= 1.0f && H < 2.0f)
            return glm::vec3(X, C, 0.0f);
        if (H >= 2.0f && H < 3.0f)
            return glm::vec3(0.0f, C, X);
        if (H >= 3.0f && H < 4.0f)
            return glm::vec3(0.0f, X, C);
        if (H >= 4.0f && H < 5.0f)
            return glm::vec3(X, 0.0f, C);
        if (H >= 5.0f && H < 6.0f)
            return glm::vec3(C, 0.0f, X);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }();

    const auto m = hslValue.z - (C / 2.0f);

    return intermediateRgb + glm::vec3(m, m, m);
}

glm::vec3 Ull::RgbToHsv(const glm::vec3 rgbValue)
{
    const auto c_max = max(max(rgbValue.r, rgbValue.g), rgbValue.b);
    const auto c_min = min(min(rgbValue.r, rgbValue.g), rgbValue.b);
    const auto delta = c_max - c_min;

    const auto H = [&]() -> float {
        if (IsZero(delta)) {
            return 0.0f;
        }
        else if (c_max == rgbValue.r) {
            const auto segment = 60.0f * ((rgbValue.g - rgbValue.b) / delta);
            if (segment < 0.0f)
                return 360.0f + segment;
            return segment;
        }
        else if (c_max == rgbValue.g) {
            return 60.0f * (((rgbValue.b - rgbValue.r) / delta) + 2.0f);
        }   
        else if (c_max == rgbValue.b) {
            return 60 * (((rgbValue.r - rgbValue.g) / delta) + 4.0f);
        }
        
        return 0.0f;
    }();

    const auto S = [&]() -> float {
        if (IsZero(c_max))
            return 0.0f;
        return delta / c_max;
    }();

    const auto V = c_max;

    return glm::vec3(H, S, V);
}

glm::vec3 Ull::HsvToRgb(const glm::vec3 hsvValue)
{
    const auto C = hsvValue.b * hsvValue.g;
    const auto X = C * (1.0f - abs(fmod(hsvValue.r / 60.0f, 2.0f) - 1.0f));
    const auto H = hsvValue.r / 60.0f;

    const auto intermediateRgb = [&](){
        if (H >= 0.0f && H < 1.0f)
            return glm::vec3(C, X, 0.0f);
        if (H >= 1.0f && H < 2.0f)
            return glm::vec3(X, C, 0.0f);
        if (H >= 2.0f && H < 3.0f)
            return glm::vec3(0.0f, C, X);
        if (H >= 3.0f && H < 4.0f)
            return glm::vec3(0.0f, X, C);
        if (H >= 4.0f && H < 5.0f)
            return glm::vec3(X, 0.0f, C);
        if (H >= 5.0f && H < 6.0f)
            return glm::vec3(C, 0.0f, X);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }();

    const auto m = hsvValue.b - C;

    return intermediateRgb + glm::vec3(m, m, m);
}
