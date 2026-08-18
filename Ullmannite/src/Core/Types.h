#pragma once
#include <variant>
#include <array>

namespace Ull
{
    typedef std::function<void(glm::vec4)> onColorChangeCallback;

    enum class State {
        Enable,
        Disable
    };

    enum class ToolType {
        ColorPicker
    };

    struct ColorPickerData {
        glm::vec4 initialColor;
        onColorChangeCallback onColorChange;
    };

    struct ToolSetup {
        ToolType toolType;
        glm::uvec2 spawnPoint;
        std::variant<ColorPickerData> toolData;
    };

    struct GradientColorData {
        float position;
        glm::vec4 color;
    };

    template<typename T>
    struct Rect {
        constexpr Rect() noexcept = default;
        constexpr Rect(T x, T y, T width, T height) noexcept
            : x{ x }, y{ y }, width{ width }, height{ height } {}

        T x = 0;
        T y = 0;
        T width = 0;
        T height = 0;

        [[nodiscard]] inline bool IsPointInside(T pointX, T pointY) const noexcept {
            return (pointX >= x) && (pointX <= x + width) && (pointY >= y) && (pointY <= y + height);
        }

        [[nodiscard]] inline bool IsPointInside(glm::vec<2, T, glm::packed_highp> point) const noexcept {
            return IsPointInside(point.x, point.y);
        }
    };

    typedef Rect<float> RectF;
    typedef Rect<unsigned> RectU;

    struct CuttingSettings
    {
        glm::vec3 cuttingPositions;
        std::array<bool, 3> invertedAxis;

        bool operator==(const CuttingSettings& cuttingSetting)
        {
            return (this->cuttingPositions == cuttingSetting.cuttingPositions && this->invertedAxis == cuttingSetting.invertedAxis);
        }

        bool operator!=(const CuttingSettings& cuttingSetting)
        {
            return !((*this) == cuttingSetting);
        }
    };
}